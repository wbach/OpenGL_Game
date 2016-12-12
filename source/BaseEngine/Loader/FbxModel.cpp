#include "FbxModel.h"

CFbxModel::CFbxModel(CTextureLoader & texture_lodaer)
: CModel()
, m_TextureLodaer(texture_lodaer)
, m_SdkManager(nullptr)
, m_Scene(nullptr)
, m_Importer(nullptr)
, m_CurrentAnimLayer(nullptr)
, m_SelectedNode(nullptr)
, m_PoseIndex(-1)
, m_Time(0)
, m_FramePerSecond(60.f)
{
}

CFbxModel::~CFbxModel()
{
	FbxArrayDelete(m_AnimStackNameArray);
	DestroySdkObjects(m_SdkManager, true);
}

void CFbxModel::InitModel(string file_name)
{
	m_FileName = file_name;
	m_FilePath = "Data/Textures/";//file_name.substr(0, file_name.find_last_of('/')) + "/";

	// initialize cache start and stop time
	m_CacheStart	= FBXSDK_TIME_INFINITE;
	m_CacheStop	= FBXSDK_TIME_MINUS_INFINITE;

	// Create the FBX SDK manager which is the object allocator for almost 
	// all the classes in the SDK and create the scene.
	try
	{
		InitializeSdkObjects(m_SdkManager, m_Scene);
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	if (m_SdkManager)
	{
		// Create the importer.
		int file_format = -1;
		m_Importer = FbxImporter::Create(m_SdkManager, "");
		if (!m_SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(m_FileName.c_str(), file_format))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			file_format = m_SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
		}

		// Initialize the importer by providing a filename.
		if (m_Importer->Initialize(m_FileName.c_str(), file_format) == false)
		{
			string error = "[Error] Cant init model : " + m_FileName;
			throw std::runtime_error(error.c_str());
		}		
	}
	LoadFile();
	SetCurrentAnimStack(0);

	if (!m_SaveBonesTransform)
		return;
	m_SavingBonesTransforms = true;
	int i = 0;
	while (1)
	{
		Update();
		m_CurrentTime += m_FrameTime;

		if (m_Stop < m_CurrentTime)
		{
			break;
		}
		i++;
	}
	m_SavingBonesTransforms = false;
	m_FrameBonesTransformsOffset = i;
	//cout << m_BoneTransformMatrixes.size() << endl;
}
void CFbxModel::InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene)
{

	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	manager = FbxManager::Create();
	if (!manager)
	{
		string error = "[Error] nable to create FBX scene ." + m_FileName;
		throw std::runtime_error(error.c_str());
	}
	//else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString path = FbxGetApplicationDirectory();
	manager->LoadPluginsDirectory(path.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	scene = FbxScene::Create(manager, "My Scene");
	if (!scene)
	{

		string error = "[Error] Unable to create FBX scene!\n : " + m_FileName;
		throw std::runtime_error(error.c_str());
	}
}
bool CFbxModel::LoadFile()
{
	bool result = false;
	// Make sure that the scene is ready to load.

	if (m_Importer->Import(m_Scene) == true)
	{
		// Set the scene status flag to refresh 
		// the scene in the first timer callback.

		// Convert Axis System to what is used in this example, if needed
		FbxAxisSystem scene_axis_system = m_Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem our_axis_system(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if (scene_axis_system != our_axis_system)
		{
			our_axis_system.ConvertScene(m_Scene);
		}

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit scene_system_unit = m_Scene->GetGlobalSettings().GetSystemUnit();
		if (scene_system_unit.GetScaleFactor() != 1.0)
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::cm.ConvertScene(m_Scene);
		}

		// Get the list of all the animation stack.
		m_Scene->FillAnimStackNameArray(m_AnimStackNameArray);

		// Get the list of all the cameras in the scene.
		// FillCameraArray(mScene, mCameraArray);

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter geom_converter(m_SdkManager);

		geom_converter.Triangulate(m_Scene, true, true);//node //<<------------------------------------------timing more..

													  // Split meshes per material, so that we only have one material per mesh (for VBO support)
		geom_converter.SplitMeshesPerMaterial(m_Scene, /*replace*/true);

		// Bake the scene for one frame
		LoadCacheRecursive(m_Scene, m_CurrentAnimLayer);

		// Initialize the frame period.
		m_FrameTime.SetTime(0, 0, 0, 1, 0, m_Scene->GetGlobalSettings().GetTimeMode());

		result = true;
	}
	else
	{
		// Import failed, set the scene status flag accordingly.
		std::cout << "[Error] Unable to import file " << m_FileName << endl;
	}

	// Destroy the importer to release the file.
	m_Importer->Destroy();
	m_Importer = nullptr;

	return result;
}
void CFbxModel::LoadCacheRecursive(FbxScene* scene, FbxAnimLayer* anim_layer)
{
	// Load the textures into GPU, only for file texture now
	const int texture_count = scene->GetTextureCount();
	for (int texture_index = 0; texture_index < texture_count; ++texture_index)
	{
		FbxTexture* texture = scene->GetTexture(texture_index);
		FbxFileTexture* file_texture = FbxCast<FbxFileTexture>(texture);
		if (file_texture && !file_texture->GetUserDataPtr())
		{
			// Try to load the texture from absolute path
			const FbxString file_name = file_texture->GetFileName();

			int texture = 0;
			std::string texture_file(file_name.Buffer());
			texture_file = m_FilePath + texture_file.substr(texture_file.find_last_of("/") + 1);

			try
			{
				texture = m_TextureLodaer.LoadTexture(texture_file); //<<---texture		
			}
			catch (const std::exception& e)
			{
				throw e;
			}
			if(texture != 0)
				file_texture->SetUserDataPtr(new int(texture));
		}
	}

	LoadCacheRecursive(scene->GetRootNode(), anim_layer);
}
void CFbxModel::LoadCacheRecursive(FbxNode* node, FbxAnimLayer* anim_layer)
{
	// Bake material and hook as user data.
	const int material_count = node->GetMaterialCount();
	for (int material_index = 0; material_index < material_count; ++material_index)
	{
		FbxSurfaceMaterial * material = node->GetMaterial(material_index);
		if (material && !material->GetUserDataPtr())
		{		
			InitializeMaterial(material);			
		}
	}

	FbxNodeAttribute* node_attribute = node->GetNodeAttribute();
	if (node_attribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (node_attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			if (mesh && !mesh->GetUserDataPtr())
			{				
				InitializeMesh(mesh);				
			}
		}
	}

	const int lChildCount = node->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadCacheRecursive(node->GetChild(lChildIndex), anim_layer);
	}

}
bool CFbxModel::InitializeMesh(FbxMesh * pMesh)
{
	if (!pMesh->GetNode())
		return false;	

	m_BonesInfo.push_back(SBonesInfo());
	SBonesInfo& bones_info = m_BonesInfo.back();

	const int lPolygonCount = pMesh->GetPolygonCount();	

	// Count the polygon count of each material
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	if (pMesh->GetElementMaterial())
	{
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();		
	}	

	FbxStringList uv_names;
	pMesh->GetUVSetNames(uv_names);
	const char* uv_name = nullptr;
	if (uv_names.GetCount())
	{		
		uv_name = uv_names[0];
	}

	// Populate the array with vertex attribute, if by control point.
	const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
	FbxVector4 current_vertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;
	
	SMaterial material;

	vector<float> postions;
	vector<float> text_coords;
	vector<float> normals;
	vector<float> tangents;
	vector<float> diffuse;
	vector<float> specular;
	vector<float> ambient;
	vector<unsigned short> indices;

	unordered_map<int, int> points;
	for (int x = 0; x < pMesh->GetControlPointsCount(); x++)
	{
		current_vertex = lControlPoints[x];
		postions.push_back(static_cast<float>(current_vertex[0]));
		postions.push_back(static_cast<float>(current_vertex[1]));
		postions.push_back(static_cast<float>(current_vertex[2]));
	}
	text_coords.resize(2 * pMesh->GetControlPointsCount());
	normals.resize(3 * pMesh->GetControlPointsCount());
	tangents.resize(3 * pMesh->GetControlPointsCount());
	indices.reserve( pMesh->GetControlPointsCount());

	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
	{
		// The material for current face.
		int lMaterialIndex = 0;
		if (lMaterialIndice)// && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{			
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
			material = m_Materials[lMaterialIndex];
			//material.diffuse = glm::vec3(1.0);
		}
	
		for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
		{
			const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);			

			current_vertex = lControlPoints[lControlPointIndex];			
			
			indices.push_back(static_cast<unsigned short>(lControlPointIndex));			

			pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
			normals[3 * lControlPointIndex] = static_cast<float>(lCurrentNormal[0]);
			normals[3 * lControlPointIndex+1] = static_cast<float>(lCurrentNormal[1]);
			normals[3 * lControlPointIndex+2] = static_cast<float>(lCurrentNormal[2]);

			bool lUnmappedUV;
			pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, uv_name, lCurrentUV, lUnmappedUV);
		
			text_coords[2*lControlPointIndex] = lCurrentUV[0];
			text_coords[2*lControlPointIndex+1] = lCurrentUV[1];		
			
			++lVertexCount;
		}
	}
	
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	lVertexCount = pMesh->GetControlPointsCount();
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	bones_info.bones.resize(lPolygonCount*3);
	int count = 0;
	unsigned int BoneIndex = 0;

	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
	{
		
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		int lClusterCount = lSkinDeformer->GetClusterCount();

		for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			string BoneName(lCluster->GetLink()->GetName());
			
			if (bones_info.bone_mapping.find(BoneName) == bones_info.bone_mapping.end())
			{
				// Allocate an index for a new bone
				BoneIndex = bones_info.num_bones;
				bones_info.num_bones++;
				SBoneInfo bi;
				bones_info.bone_info.push_back(bi);
				bones_info.bone_info[BoneIndex].BoneOffset = glm::mat4(0.0f);
				bones_info.bone_mapping[BoneName] = BoneIndex;
			}
			else
			{
				BoneIndex = bones_info.bone_mapping[BoneName];
			}	

			for (int k = 0; k < lCluster->GetControlPointIndicesCount(); ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
					continue;	


				if (lClusterMode == FbxCluster::eAdditive)
				{		
					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lWeight = 1.0;
				}				
				bones_info.bones[lIndex].AddBoneData(BoneIndex, lWeight);
			}
		}
	}
	CMesh* mesh = AddMesh("FBX mesh", postions, text_coords, normals, tangents, indices, material, bones_info.bones);
	pMesh->SetUserDataPtr(mesh);
	return false;
}

bool CFbxModel::InitializeMaterial(const FbxSurfaceMaterial * pMaterial)
{
	SMaterial material;
	int* ambient_textur = nullptr;
	const FbxDouble3 ambient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, ambient_textur);
	material.ambient.x = static_cast<GLfloat>(ambient[0]);
	material.ambient.y = static_cast<GLfloat>(ambient[1]);
	material.ambient.z = static_cast<GLfloat>(ambient[2]);

	if (ambient_textur != nullptr)
	{
		m_TextureLodaer.GetTexture(*ambient_textur)->type = MaterialTexture::AMBIENT;
		material.textures.push_back(*ambient_textur);
	}
	
	int* diffuse_textur = nullptr;
	const FbxDouble3 diffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, diffuse_textur);
	material.diffuse.x = static_cast<GLfloat>(diffuse[0]);
	material.diffuse.y = static_cast<GLfloat>(diffuse[1]);
	material.diffuse.z = static_cast<GLfloat>(diffuse[2]);

	if (diffuse_textur != nullptr)
	{
		m_TextureLodaer.GetTexture(*diffuse_textur)->type = MaterialTexture::DIFFUSE;
		material.textures.push_back(*diffuse_textur);
	}	

	int* specular_textur = nullptr;
	const FbxDouble3 specular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, specular_textur);
	material.specular.x = static_cast<GLfloat>(specular[0]);
	material.specular.y = static_cast<GLfloat>(specular[1]);
	material.specular.z = static_cast<GLfloat>(specular[2]);

	if (specular_textur != nullptr)
	{
		//specular_textur->type = MaterialTexture::SPECULAR;
		m_TextureLodaer.GetTexture(*specular_textur)->type = MaterialTexture::SPECULAR;
		material.textures.push_back(*specular_textur);
	}	

	FbxProperty shininess_property = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininess_property.IsValid())
	{
		double shininess = shininess_property.Get<FbxDouble>();
		material.shineDamper = static_cast<GLfloat>(shininess);
	}
	m_Materials.push_back(material);
	return true;
}

bool CFbxModel::SetCurrentAnimStack(int index)
{
	const int lAnimStackCount = m_AnimStackNameArray.GetCount();
	if (!lAnimStackCount || index >= lAnimStackCount)
	{
		return false;
	}

	// select the base layer from the animation stack
	FbxAnimStack * lCurrentAnimationStack = m_Scene->FindMember<FbxAnimStack>(m_AnimStackNameArray[index]->Buffer());
	if (lCurrentAnimationStack == NULL)
	{
		// this is a problem. The anim stack should be found in the scene!
		return false;
	}

	// we assume that the first animation layer connected to the animation stack is the base layer
	// (this is the assumption made in the FBXSDK)
	m_CurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	m_Scene->SetCurrentAnimationStack(lCurrentAnimationStack);

	FbxTakeInfo* lCurrentTakeInfo = m_Scene->GetTakeInfo(*(m_AnimStackNameArray[index]));
	if (lCurrentTakeInfo)
	{
		m_Start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		m_Stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	}
	else
	{
		// Take the time line value
		FbxTimeSpan lTimeLineTimeSpan;
		m_Scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

		m_Start = lTimeLineTimeSpan.GetStart();
		m_Stop = lTimeLineTimeSpan.GetStop();
	}

	// check for smallest start with cache start
	if (m_CacheStart < m_Start)
		m_Start = m_CacheStart;

	// check for biggest stop with cache stop
	if (m_CacheStop  > m_Stop)
		m_Stop = m_CacheStop;

	// move to beginning
	m_CurrentTime = m_Start;

	return true;
}
void CFbxModel::Update(float delta_time)
{	
	m_Time += delta_time;
	if (m_Time < (1.f / m_FramePerSecond))	
		return;	
	m_Time = 0;
	
	OnTimerClick();

	for (const auto& i : m_CurrentFrames)
	{
		(*i)++;
		if ((*i) > m_FrameBonesTransformsOffset)
			(*i) = 0;
	}

	if (m_SaveBonesTransform)
		return;
	Update();
}
void CFbxModel::Update()
{
	FbxPose* pose = nullptr;
	if (m_PoseIndex != -1)
	{
		pose = m_Scene->GetPose(m_PoseIndex);
	}
	// If one node is selected, draw it and its children.
	FbxAMatrix lDummyGlobalPosition;

	UpdateNodeRecursive(m_Scene->GetRootNode(), m_CurrentTime, m_CurrentAnimLayer, lDummyGlobalPosition, pose);
}
void CFbxModel::UpdateNodeRecursive(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer,	FbxAMatrix& parent_global_position, FbxPose* pose)
{

	FbxAMatrix lGlobalPosition = GetGlobalPosition(node, time, pose, &parent_global_position);

	if (node->GetNodeAttribute())
	{
		// Geometry offset.
		// it is not inherited by the children.
		FbxAMatrix lGeometryOffset = GetGeometry(node);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition * lGeometryOffset;

		UpdateNode(node, time, anim_layer, parent_global_position, lGlobalOffPosition, pose);
	}

	const int lChildCount = node->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		UpdateNodeRecursive(node->GetChild(lChildIndex), time, anim_layer, lGlobalPosition, pose);
	}

}
void CFbxModel::UpdateNode(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer, FbxAMatrix& parent_global_position, FbxAMatrix& global_position, FbxPose* pose)
{
	FbxNodeAttribute* lNodeAttribute = node->GetNodeAttribute();
	if (lNodeAttribute)
	{
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			UpdateMesh(node, time, anim_layer, global_position, pose);
		}
	}
}
void CFbxModel::UpdateMesh(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer, FbxAMatrix& global_position, FbxPose* pose)
{
	FbxMesh* lMesh = node->GetMesh();
	const int lVertexCount = lMesh->GetControlPointsCount();

	// No vertex to draw.
	if (lVertexCount == 0)
	{
		return;
	}

	const CMesh * lMeshCache = static_cast<const CMesh*>(lMesh->GetUserDataPtr());

	// If it has some defomer connection, update the vertices position
	const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active;
	const bool lHasShape = lMesh->GetShapeCount() > 0;
	const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

	FbxVector4* lVertexArray = NULL;
	if (!lMeshCache || lHasDeformation)
	{
		//lVertexArray = new FbxVector4[lVertexCount];
		//memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
	}

	if (lHasDeformation)
	{
		// Active vertex cache deformer will overwrite any other deformer
		if (lHasVertexCache)
		{
			ReadVertexCacheData(lMesh, time, lVertexArray);
		}
		else
		{
			if (lHasShape)
			{
				// Deform the vertex array with the shapes.
				ComputeShapeDeformation(lMesh, time, anim_layer, lVertexArray);
			}

			//we need to get the number of clusters
			const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
			int lClusterCount = 0;
			for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
			{
				lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
			}
			if (lClusterCount)
			{
				// Deform the vertex array with the skin deformer.
				ComputeSkinDeformation(global_position, lMesh, time, lVertexArray, pose);
			}
		}		
	}

//	delete[] lVertexArray;
}
void CFbxModel::ReadVertexCacheData(FbxMesh* mesh, FbxTime& time, FbxVector4* verttex_array)
{

	FbxVertexCacheDeformer*  deformer	  = static_cast<FbxVertexCacheDeformer*>(mesh->GetDeformer(0, FbxDeformer::eVertexCache));
	FbxCache*                cache		  = deformer->GetCache();
	int                      chanel_index = -1;
	unsigned int             vertex_count = (unsigned int)mesh->GetControlPointsCount();
	bool                     read_succeed = false;
	double*                  read_buf     = new double[3 * vertex_count];

	if (cache->GetCacheFileFormat() == FbxCache::eMayaCache)
	{
		if ((chanel_index = cache->GetChannelIndex(deformer->Channel.Get())) > -1)
		{
			read_succeed = cache->Read(chanel_index, time, read_buf, vertex_count);
		}
	}
	else // eMaxPointCacheV2
	{
		read_succeed = cache->Read((unsigned int)time.GetFrameCount(), read_buf, vertex_count);
	}

	if (read_succeed)
	{
		unsigned int read_buf_index = 0;

		while (read_buf_index < 3 * vertex_count)
		{
			// In statements like "pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex++])", 
			// on Mac platform, "lReadBufIndex++" is evaluated before "lReadBufIndex/3". 
			// So separate them.
			verttex_array[read_buf_index / 3].mData[0] = read_buf[read_buf_index]; read_buf_index++;
			verttex_array[read_buf_index / 3].mData[1] = read_buf[read_buf_index]; read_buf_index++;
			verttex_array[read_buf_index / 3].mData[2] = read_buf[read_buf_index]; read_buf_index++;
		}
	}

	delete[] read_buf;

}
void CFbxModel::ComputeShapeDeformation(FbxMesh* mesh, FbxTime& time, FbxAnimLayer* anim_layer, FbxVector4* vertex_array)
{
	int vertex_count = mesh->GetControlPointsCount();

	FbxVector4* src_vertex_array = vertex_array;
	FbxVector4* dst_vertex_array = new FbxVector4[vertex_count];
	memcpy(dst_vertex_array, vertex_array, vertex_count * sizeof(FbxVector4));

	int blend_shape_deformer_count = mesh->GetDeformerCount(FbxDeformer::eBlendShape);
	for (int blend_shape_index = 0; blend_shape_index < blend_shape_deformer_count; ++blend_shape_index)
	{
		FbxBlendShape* blend_shape = (FbxBlendShape*)mesh->GetDeformer(blend_shape_index, FbxDeformer::eBlendShape);

		int blend_shape_channel_count = blend_shape->GetBlendShapeChannelCount();
		for (int channel_index = 0; channel_index < blend_shape_channel_count; ++channel_index)
		{
			FbxBlendShapeChannel* channel = blend_shape->GetBlendShapeChannel(channel_index);
			if (channel)
			{
				// Get the percentage of influence on this channel.
				FbxAnimCurve* fcurve = mesh->GetShapeChannel(blend_shape_index, channel_index, anim_layer);
				if (!fcurve) continue;
				double weight = fcurve->Evaluate(time);

				/*
				If there is only one targetShape on this channel, the influence is easy to calculate:
				influence = (targetShape - baseGeometry) * weight * 0.01
				dstGeometry = baseGeometry + influence

				But if there are more than one targetShapes on this channel, this is an in-between
				blendshape, also called progressive morph. The calculation of influence is different.

				For example, given two in-between targets, the full weight percentage of first target
				is 50, and the full weight percentage of the second target is 100.
				When the weight percentage reach 50, the base geometry is already be fully morphed
				to the first target shape. When the weight go over 50, it begin to morph from the
				first target shape to the second target shape.

				To calculate influence when the weight percentage is 25:
				1. 25 falls in the scope of 0 and 50, the morphing is from base geometry to the first target.
				2. And since 25 is already half way between 0 and 50, so the real weight percentage change to
				the first target is 50.
				influence = (firstTargetShape - baseGeometry) * (25-0)/(50-0) * 100
				dstGeometry = baseGeometry + influence

				To calculate influence when the weight percentage is 75:
				1. 75 falls in the scope of 50 and 100, the morphing is from the first target to the second.
				2. And since 75 is already half way between 50 and 100, so the real weight percentage change
				to the second target is 50.
				influence = (secondTargetShape - firstTargetShape) * (75-50)/(100-50) * 100
				dstGeometry = firstTargetShape + influence
				*/

				// Find the two shape indices for influence calculation according to the weight.
				// Consider index of base geometry as -1.

				int shape_count		 = channel->GetTargetShapeCount();
				double* full_weights = channel->GetTargetShapeFullWeights();

				// Find out which scope the lWeight falls in.
				int start_index = -1;
				int end_index   = -1;
				for (int shape_index = 0; shape_index < shape_count; ++shape_index)
				{
					if (weight > 0 && weight <= full_weights[0])
					{
						end_index = 0;
						break;
					}
					if (weight > full_weights[shape_index] && weight < full_weights[shape_index + 1])
					{
						start_index = shape_index;
						end_index   = shape_index + 1;
						break;
					}
				}

				FbxShape* start_shape = NULL;
				FbxShape* end_shape   = NULL;
				if (start_index > -1)
				{
					start_shape = channel->GetTargetShape(start_index);
				}
				if (end_index > -1)
				{
					end_shape = channel->GetTargetShape(end_index);
				}

				//The weight percentage falls between base geometry and the first target shape.
				if (start_index == -1 && end_shape)
				{
					double end_weight = full_weights[0];
					// Calculate the real weight.
					weight = (weight / end_weight) * 100;
					// Initialize the lDstVertexArray with vertex of base geometry.
					memcpy(dst_vertex_array, src_vertex_array, vertex_count * sizeof(FbxVector4));
					for (int j = 0; j < vertex_count; j++)
					{
						// Add the influence of the shape vertex to the mesh vertex.
						FbxVector4 influence = (end_shape->GetControlPoints()[j] - src_vertex_array[j]) * weight * 0.01;
						dst_vertex_array[j] += influence;
					}
				}
				//The weight percentage falls between two target shapes.
				else if (start_shape && end_shape)
				{
					double start_weight = full_weights[start_index];
					double end_weight   = full_weights[end_index];
					// Calculate the real weight.
					weight = ((weight - start_weight) / (end_weight - start_weight)) * 100;
					// Initialize the lDstVertexArray with vertex of the previous target shape geometry.
					memcpy(dst_vertex_array, start_shape->GetControlPoints(), vertex_count * sizeof(FbxVector4));
					for (int j = 0; j < vertex_count; j++)
					{
						// Add the influence of the shape vertex to the previous shape vertex.
						FbxVector4 influence = (end_shape->GetControlPoints()[j] - start_shape->GetControlPoints()[j]) * weight * 0.01;
						dst_vertex_array[j] += influence;
					}
				}
			}//If lChannel is valid
		}//For each blend shape channel
	}//For each blend shape deformer

	memcpy(vertex_array, dst_vertex_array, vertex_count * sizeof(FbxVector4));

	delete[] dst_vertex_array;

}
void CFbxModel::ComputeSkinDeformation(FbxAMatrix& global_position, FbxMesh* mesh, FbxTime& time, FbxVector4* vertex_vrray, FbxPose* pose)
{

	FbxSkin * skin_deformer = (FbxSkin *)mesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType skinning_type = skin_deformer->GetSkinningType();
	if (skinning_type == FbxSkin::eLinear || skinning_type == FbxSkin::eRigid)
	{	
		ComputeLinearDeformation(global_position, mesh, time, vertex_vrray, pose);
	}
	else if (skinning_type == FbxSkin::eDualQuaternion)
	{		
		ComputeDualQuaternionDeformation(global_position, mesh, time, vertex_vrray, pose);
	}
	else if (skinning_type == FbxSkin::eBlend)
	{
		int lVertexCount = mesh->GetControlPointsCount();

		FbxVector4* vertex_array_linear = new FbxVector4[lVertexCount];
		memcpy(vertex_array_linear, mesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		FbxVector4* vertex_array_dq = new FbxVector4[lVertexCount];
		memcpy(vertex_array_dq, mesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		ComputeLinearDeformation(global_position, mesh, time, vertex_array_linear, pose);
		ComputeDualQuaternionDeformation(global_position, mesh, time, vertex_array_dq, pose);

		// To blend the skinning according to the blend weights
		// Final vertex = DQSVertex * blend weight + LinearVertex * (1- blend weight)
		// DQSVertex: vertex that is deformed by dual quaternion skinning method;
		// LinearVertex: vertex that is deformed by classic linear skinning method;
		int blend_weights_count = skin_deformer->GetControlPointIndicesCount();
		
		for (int bw_index = 0; bw_index < blend_weights_count; ++bw_index)
		{
			double blend_weight = skin_deformer->GetControlPointBlendWeights()[bw_index];
			vertex_vrray[bw_index] = vertex_array_dq[bw_index] * blend_weight + vertex_array_linear[bw_index] * (1 - blend_weight);
		}
	}

}
void CFbxModel::ComputeLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose)
{
	//m_BoneTransformMatrixes[0].clear();
	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.

	m_FrameBonesTransforms = 0;
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int lSkinIndex = 0; lSkinIndex<lSkinCount; ++lSkinIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);

		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);
			unsigned int id = 0;

			if (m_SavingBonesTransforms)
			{
				m_BoneTransformMatrixes.push_back(FbxMatrixToGlm(lVertexTransformMatrix));
				m_FrameBonesTransforms++;
			}
			else
			{
				if (m_BoneTransformMatrixes.size() < m_FrameBonesTransforms + 1) m_BoneTransformMatrixes.push_back(glm::mat4(1.f));
				m_BoneTransformMatrixes[m_FrameBonesTransforms++] = (FbxMatrixToGlm(lVertexTransformMatrix));
			}			
		}
	}


	//cout << m_BoneTransformMatrixes[0].size() << endl;
}
void CFbxModel::ComputeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose)
{

	// All the links must have the same link mode.
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	FbxDualQuaternion* lDQClusterDeformation = new FbxDualQuaternion[lVertexCount];
	memset(lDQClusterDeformation, 0, lVertexCount * sizeof(FbxDualQuaternion));

	double* lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

	// For all skins and all clusters, accumulate their deformation and weight
	// on each vertices and store them in lClusterDeformation and lClusterWeight.
	for (int lSkinIndex = 0; lSkinIndex<lSkinCount; ++lSkinIndex)
	{
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex<lClusterCount; ++lClusterIndex)
		{
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink())
				continue;

			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

			FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
			FbxVector4 lT = lVertexTransformMatrix.GetT();
			FbxDualQuaternion lDualQuaternion(lQ, lT);

			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (lIndex >= lVertexCount)
					continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0)
					continue;

				// Compute the influence of the link on the vertex.
				FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
				if (lClusterMode == FbxCluster::eAdditive)
				{
					// Simply influenced by the dual quaternion.
					lDQClusterDeformation[lIndex] = lInfluence;

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					if (lClusterIndex == 0)
					{
						lDQClusterDeformation[lIndex] = lInfluence;
					}
					else
					{
						// Add to the sum of the deformations on the vertex.
						// Make sure the deformation is accumulated in the same rotation direction. 
						// Use dot product to judge the sign.
						double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
						if (lSign >= 0.0)
						{
							lDQClusterDeformation[lIndex] += lInfluence;
						}
						else
						{
							lDQClusterDeformation[lIndex] -= lInfluence;
						}
					}
					// Add to the sum of weights to either normalize or complete the vertex.
					lClusterWeight[lIndex] += lWeight;
				}
			}//For each vertex
		}//lClusterCount
	}

	//Actually deform each vertices here by information stored in lClusterDeformation and lClusterWeight
	for (int i = 0; i < lVertexCount; i++)
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeightSum = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (lWeightSum != 0.0)
		{
			lDQClusterDeformation[i].Normalize();
			lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeightSum;
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeightSum);
				lDstVertex += lSrcVertex;
			}
		}
	}

	delete[] lDQClusterDeformation;
	delete[] lClusterWeight;

}
void CFbxModel::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose)
{

	FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();

	FbxAMatrix lReferenceGlobalInitPosition;
	FbxAMatrix lReferenceGlobalCurrentPosition;
	FbxAMatrix lAssociateGlobalInitPosition;
	FbxAMatrix lAssociateGlobalCurrentPosition;
	FbxAMatrix lClusterGlobalInitPosition;
	FbxAMatrix lClusterGlobalCurrentPosition;

	FbxAMatrix lReferenceGeometry;
	FbxAMatrix lAssociateGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;

	if (lClusterMode == FbxCluster::eAdditive && pCluster->GetAssociateModel())
	{
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);
		// Geometric transform of the model
		lAssociateGeometry = GetGeometry(pCluster->GetAssociateModel());
		lAssociateGlobalInitPosition *= lAssociateGeometry;
		lAssociateGlobalCurrentPosition = GetGlobalPosition(pCluster->GetAssociateModel(), pTime, pPose);

		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;
		lReferenceGlobalCurrentPosition = pGlobalPosition;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		// Multiply lClusterGlobalInitPosition by Geometric Transformation
		lClusterGeometry = GetGeometry(pCluster->GetLink());
		lClusterGlobalInitPosition *= lClusterGeometry;
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
			lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;
	}
	else
	{
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		lReferenceGeometry = GetGeometry(pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose);

		// Compute the initial position of the link relative to the reference.
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}

}
void CFbxModel::MatrixScale(FbxAMatrix& pMatrix, double pValue)
{
	int i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pMatrix[i][j] *= pValue;
		}
	}
}
void CFbxModel::MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue)
{
	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}
void CFbxModel::MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix)
{
	int i, j;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			pDstMatrix[i][j] += pSrcMatrix[i][j];
		}
	}
}

FbxAMatrix CFbxModel::GetGlobalPosition(FbxNode* node, const FbxTime& time, FbxPose* pose, FbxAMatrix* parent_global_position)
{

	FbxAMatrix lGlobalPosition;
	bool        lPositionFound = false;

	if (pose)
	{
		int lNodeIndex = pose->Find(node);

		if (lNodeIndex > -1)
		{
			// The bind pose is always a global matrix.
			// If we have a rest pose, we need to check if it is
			// stored in global or local space.
			if (pose->IsBindPose() || !pose->IsLocalMatrix(lNodeIndex))
			{
				lGlobalPosition = GetPoseMatrix(pose, lNodeIndex);
			}
			else
			{
				// We have a local matrix, we need to convert it to
				// a global space matrix.
				FbxAMatrix lParentGlobalPosition;

				if (parent_global_position)
				{
					lParentGlobalPosition = *parent_global_position;
				}
				else
				{
					if (node->GetParent())
					{
						lParentGlobalPosition = GetGlobalPosition(node->GetParent(), time, pose);
					}
				}

				FbxAMatrix lLocalPosition = GetPoseMatrix(pose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}

			lPositionFound = true;
		}
	}

	if (!lPositionFound)
	{
		// There is no pose entry for that node, get the current global position instead.

		// Ideally this would use parent global position and local position to compute the global position.
		// Unfortunately the equation 
		//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
		// does not hold when inheritance type is other than "Parent" (RSrs).
		// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		lGlobalPosition = node->EvaluateGlobalTransform(time);
	}

	return lGlobalPosition;
}
FbxAMatrix CFbxModel::GetPoseMatrix(FbxPose* pose, int node_index)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix = pose->GetMatrix(node_index);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}
FbxAMatrix CFbxModel::GetGeometry(FbxNode* node)
{
	const FbxVector4 lt = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lr = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 ls = node->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lt, lr, ls);
}
const string& CFbxModel::GetName() const
{
	return m_Name;
}

void CFbxModel::OnTimerClick()
{
	// Loop in the animation stack if not paused.
	if (m_Stop > m_Start)
	{		
		m_CurrentTime += m_FrameTime;
		m_CurrentFrameBones += 1;
		
		if (m_CurrentTime > m_Stop)
		{
			m_CurrentTime = m_Start;
			m_CurrentFrameBones = 0;
		}
	}
}

glm::mat4 CFbxModel::FbxMatrixToGlm(FbxAMatrix & mx)
{
	glm::mat4 m;
	m[0][0] = mx[0][0]; m[0][1] = mx[0][1]; m[0][2] = mx[0][2]; m[0][3] = mx[0][3];
	m[1][0] = mx[1][0]; m[1][1] = mx[1][1]; m[1][2] = mx[1][2]; m[1][3] = mx[1][3];
	m[2][0] = mx[2][0]; m[2][1] = mx[2][1]; m[2][2] = mx[2][2]; m[2][3] = mx[2][3];
	m[3][0] = mx[3][0]; m[3][1] = mx[3][1]; m[3][2] = mx[3][2]; m[3][3] = mx[3][3];
	return m;
}
void CFbxModel::UnloadCacheRecursive(FbxNode * pNode)
{
	// Unload the material cache
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && lMaterial->GetUserDataPtr())
		{
			SMaterial* lMaterialCache = static_cast<SMaterial *>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(NULL);
			delete lMaterialCache;
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Unload the mesh cache
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh * lMesh = pNode->GetMesh();
			if (lMesh && lMesh->GetUserDataPtr())
			{
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		UnloadCacheRecursive(pNode->GetChild(lChildIndex));
	}
}
void CFbxModel::UnloadCacheRecursive(FbxScene * pScene)
{
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	{
		FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && lFileTexture->GetUserDataPtr())
		{
		}
	}

	UnloadCacheRecursive(pScene->GetRootNode());
}
void CFbxModel::DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (pManager) pManager->Destroy();
}

FbxDouble3 CFbxModel::GetMaterialProperty(const FbxSurfaceMaterial* material, const char* property_name, const char* factor_property_name, int*& texture_id)
{
	texture_id = nullptr;
	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = material->FindProperty(property_name);
	const FbxProperty lFactorProperty = material->FindProperty(factor_property_name);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if (lProperty.IsValid())
	{
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture && lTexture->GetUserDataPtr())
			{
				texture_id = (static_cast<int*>(lTexture->GetUserDataPtr()));
			}		
		}
	}

	return lResult;
}