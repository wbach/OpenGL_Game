#include "Scene.h"
#include "GLgame.h"
CScene::CScene(CGame* game) 
: m_Game(game)
, m_CurrentTerrain(nullptr)
, m_TerrainsCount(5)
, m_TerrainViewRadius(1)
, m_GloabalTime(0.f)
, m_DirectionalLight(glm::vec3(10000,15000,10000), glm::vec3(0.01))
, m_DayNightCycle(m_DirectionalLight)
, m_GUI(nullptr)
{
	m_Loader.SetMaxTextureResolution(m_Game->GetMaxTextureResolution());
}

void CScene::AddSkyBoxTexture(int type, std::string texture)
{
	if (type == 0)
		m_DaySkyboxTextures.push_back(texture);
	else
		m_NightSkyboxTextures.push_back(texture);
}

void CScene::AddSkyBoxCubeTexture(int type, vector<string> texture)
{
	if (type == 0)
		m_DaySkyboxTextures = texture;
	else
		m_NightSkyboxTextures = texture;
}

void CScene::LoadSkyBox()
{
	if (m_DaySkyboxTextures.size() == 6 && m_NightSkyboxTextures.size() == 6)
	{
		try
		{
			int day_texture = m_Loader.LoadCubeMap(m_DaySkyboxTextures);
			int night_texture = m_Loader.LoadCubeMap(m_NightSkyboxTextures);
			m_Game->GetMasterRenderer().SetSkyBoxTextures(m_Loader.m_Textures[day_texture].get(), m_Loader.m_Textures[night_texture].get());
			int skybox_cube = m_Loader.LoadMesh("Data/Meshes/SkyBox/cube.obj");
			if (skybox_cube >= 0)
				m_Game->GetMasterRenderer().SetSkyBoxMesh(&m_Loader.m_Models[skybox_cube]->GetMeshes()[0]);
		}
		catch (const std::runtime_error& e)
		{
			throw e;
		}		
	}
}
//
//void CScene::AddTerrain(CTerrain terrain, int x, int z) 
//{ 
//	if (terrain.GetName().size() <=0 || terrain.GetName().compare("No name terrain") == 0)
//		terrain.SetName(string("Terrain_") + to_string(x) + "x" + to_string(z) );
//	m_Terrains[x][z] = terrain;
//}

void CScene::AddGameObject(shared_ptr<CGameObject> game_object, bool direct, bool interacted)
{ 
	if (direct)
	{
		m_GameObjects.push_back(game_object);
		if (interacted)
			m_InteractedObjects.push_back(m_GameObjects.back().get());
		return;
	}
	int xx, zz;
	TerrainNumber(game_object->GetPositionXZ(), xx, zz);
	if (xx >= 0 && zz >= 0)
	{
		CTerrain* terrain = GetTerrain(xx, zz);
		if(terrain != nullptr)
			GetTerrain(xx, zz)->AddSubbObject(game_object, interacted);
		else
		{
			m_GameObjects.push_back(game_object);
			if (interacted)
				m_InteractedObjects.push_back(m_GameObjects.back().get());
		}
			
	}		
	else
	{
		m_GameObjects.push_back(game_object);
		if (interacted)
			m_InteractedObjects.push_back(m_GameObjects.back().get());
	}
}

void CScene::SaveHeightMaps()
{
	//for (const CTerrain& terrain : m_Terrains)
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount; x++)
		{
			CTerrain* terrain = GetTerrain(x, y);
			if (terrain == nullptr)
				continue;
			std::ofstream file;
			string fn = terrain->m_HeightMapPath;
			file.open(fn);
			string size = "r" + to_string(terrain->m_HeightMapResolution) + "x" + to_string(terrain->m_HeightMapResolution);
			file << size << endl;
			for (int y = 0; y < terrain->m_HeightMapResolution; y++)
			{
				for (int x = 0; x < terrain->m_HeightMapResolution; x++)
				{
					file << terrain->GetHeight(x, y) << " ";
				}
				file << endl;
			}
			file.close();
		}	
}

void CScene::SaveBlendMaps()
{
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount ; x++)
		{
			CTerrain& terrain = *GetTerrain(x, y);
			m_Loader.SaveTextureToFile(terrain.m_BlendMapPath, terrain.m_BlendMapData, terrain.m_BlendMapWidth, terrain.m_BlendMapHeight);
		}	
}

void CScene::MergeTerrains(CTerrain & t1, CTerrain & t2, int axis)
{
	if (t1.m_HeightMapResolution != t2.m_HeightMapResolution) return;

	for (int x = 0; x < t1.m_HeightMapResolution; x++)
	{
		if (axis == 0)
		{
			float height = t1.GetHeight(t1.m_HeightMapResolution - 1, x) + t2.GetHeight(0, x);
			height /= 2;
			t1.SetHeight(t1.m_HeightMapResolution - 1, x, height);
			t2.SetHeight(0, x, height);
		}
		else
		{
			float height = t1.GetHeight(x, t1.m_HeightMapResolution - 1) + t2.GetHeight(x, 0);
			height /= 2;
			t1.SetHeight(x, t1.m_HeightMapResolution - 1, height);
			t2.SetHeight(x, 0, height);
		}
	}
	t1.ReloadVertex();
	t2.ReloadVertex();
}

void CScene::MergeAllTerrains()
{
	for (int y = 0; y < m_TerrainsCount; y++)
		for (int x = 0; x < m_TerrainsCount; x++)
		{
			CTerrain* t1 = GetTerrain(x, y);
			CTerrain* t2 = GetTerrain(x + 1, y);
			CTerrain* t3 = GetTerrain(x, y + 1);
			if(t1 != nullptr && t2 != nullptr)
				MergeTerrains(*t1, *t2, 0);
			if (t1 != nullptr && t3 != nullptr)
				MergeTerrains(*t1, *t3, 1);
		}
}

CTerrain* CScene::FindTerrainByName(string name)
{
	
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount; x++)
		{
			CTerrain& terrain = *GetTerrain(x, y);
			if (!terrain.GetName().compare(name))
				return &terrain;
		}
	
	return nullptr;
}

void CScene::ApplyPhysicsToObjects(float dt)
{
	//return;
	unsigned int id = 0; 
	std::list<CRigidbody> bodys;
	for (const auto& entity : m_PhysicsEntities)
	{
		if (abs(glm::length(entity->GetWorldPosition() - m_Camera->GetPosition()) ) < 75)
			bodys.push_back(entity->m_RigidBody);
	}
	for (const auto& entity : m_PhysicsEntities)
	{		
		if (entity->m_RigidBody.m_Static)
			continue;
		entity->m_RigidBody.ApplyGravity(dt, -9.81f);

		entity->IncrasePosition(entity->m_RigidBody.m_AngularVelocity * dt);
		entity->m_RigidBody.SetPosition(entity->GetWorldPosition() + glm::vec3(0, 0.5, 0));

		bool grounded = false;
		
		std::list<SCollisionInfo> col_list = entity->m_RigidBody.CheckCollsions(bodys, id);

		for (const auto& col : col_list)
		{
			entity->IncrasePosition(col.updateVector);
			if (glm::dot(glm::vec3(0, 1, 0), col.normal) > 0.75f)
			{
				entity->m_RigidBody.m_AngularVelocity.y = 0;
				grounded = true;
			}				
		}		

		float h = 0;		
		h = GetHeightOfTerrain(entity->GetPositionXZ());

		if (entity->GetWorldPosition().y < h )
		{
			entity->SetPositionY(h);
			entity->m_RigidBody.m_AngularVelocity.y = 0;
		//	entity->m_RigidBody.ReflectVelocity(glm::vec3(0, 1, 0));
			grounded = true;
		}
		entity->m_RigidBody.m_IsGrounded = grounded;
		id++;
	}
}

void CScene::ClearObjectsVelocity()
{
	for (auto& entity : m_PhysicsEntities)
	{
		float h = GetHeightOfTerrain(entity->GetPositionXZ());
		entity->SetPositionY(h);
		entity->m_RigidBody.m_AngularVelocity = glm::vec3(0, 0, 0);
		entity->m_RigidBody.SetPosition(entity->GetWorldPosition() + glm::vec3(0,50,0));
	}
}

void CScene::DeleteEntity(shared_ptr<CEntity>& deleted_entity)
{
	//bool deleted = false;
	////for (CTerrain& terrain : m_Terrains)
	//for (unsigned int y = 0; y < m_TerrainsCount; y++)
	//	for (unsigned int x = 0; x < m_TerrainsCount; x++)
	//{
	//	CTerrain& terrain = *GetTerrain(x, y);
	//	int i = 0;
	//	for (auto& entity : terrain.m_TerrainEntities)
	//	{
	//		if (entity->GetId() == deleted_entity->GetId() )
	//		{
	//			terrain.m_TerrainEntities.erase(terrain.m_TerrainEntities.begin() + i);
	//			return;
	//		}
	//		deleted = DeleteSubEntity(entity, deleted_entity->GetId());
	//		if (deleted)
	//			return;
	//		i++;
	//	}
	//}
	//int i = 0;
	//for (auto& entity : m_Entities)
	//{
	//	if (entity->GetId() == deleted_entity->GetId())
	//	{
	//		m_Entities.erase(m_Entities.begin() + i);
	//		return;
	//	}
	//	deleted = DeleteSubEntity(entity, deleted_entity->GetId());
	//	if (deleted)
	//		return;
	//	i++;
	//}
}
bool CScene::DeleteSubEntity(shared_ptr<CEntity>& entity, int id)
{
	//bool deleted = false;
	//int i = 0;
	//for (auto& subentity : entity->GetChildrenEntities())
	//{
	//	if (subentity->GetId() == id)
	//	{
	//		//entity->GetChildrenEntities().erase(entity->GetChildrenEntities().begin() + i);
	//		return true;
	//	}
	//	deleted = DeleteSubEntity(subentity, id);
	//	if (deleted)
	//		return true;
	//	i++;
	//}
	return false;
}
shared_ptr<CEntity> CScene::CreateEntityFromFile(string file_name, const ColiderType::Type type, glm::vec3 normalized_size , bool instanced, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const glm::mat4& parent_matrix)
{	
	std::string filename = Utils::ConvertToRelativePath(file_name);
	std::string name = filename.substr(filename.find_last_of('/') + 1);
	std::string path = filename.substr(0, filename.find_last_of('/') + 1);
	std::string col_fname = path + "collisions_" + name;
	//file_name = col_fname;
	shared_ptr<CEntity> new_entity;
	new_entity = make_shared<CEntity>(pos, rot, scale);		
	new_entity->SetRelativeMatrix(parent_matrix);
	new_entity->CalculateTransformMatrix();
	int model_id = -1;

	try
	{
		model_id = m_Loader.LoadMesh(file_name);
	}
	catch (std::exception& e)
	{
		cout << e.what();
	}
	

	if (model_id == -1) return nullptr;

	if(glm::length(normalized_size) > 0.001)
	{
		new_entity->SetNormalizedSize(normalized_size);
		glm::mat4 normalized_matrix = m_Loader.m_Models[model_id]->CalculateNormalizedMatrix(normalized_size.x, normalized_size.y, normalized_size.z);
		new_entity->SetNormalizedMatrix(normalized_matrix);
	}	

	new_entity->SetBoundingSize(m_Loader.m_Models[model_id]->GetBoundingMaxSize());
	new_entity->SetModelId(model_id);

	if(instanced)
		m_Loader.m_Models[model_id]->CreateTransformsVbo(new_entity->GetTransformMatrixes());

	new_entity->SetName(name);
	new_entity->SetFullPath(file_name);

	switch (type)
	{
		case ColiderType::MESH_COLIDER:
		{
			if (!Utils::CheckFile(col_fname))
			{
				std::cout << "[Error] The file " << col_fname << " wasnt successfuly opened " << std::endl;
				break;
			}

			vector<SFace> faces = m_Loader.LoadFaces(col_fname);
			new_entity->m_RigidBody = CRigidbody(faces);
			//new_entity->CalculateEntityTransformMatrix();
			//Utils::PrintMatrix("Mat: ", new_entity->GetTransformMatrix());			
			new_entity->m_RigidBody.m_Colider.TransformFaces(new_entity->GetRelativeTransformMatrix()* new_entity->GetTransformMatrix());
			m_PhysicsEntities.push_back(new_entity.get());
		}
		break;
		case ColiderType::SPHERE_COLIDER:
			new_entity->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 0.5));
			new_entity->m_RigidBody.m_Static = false;
			m_PhysicsEntities.push_back(new_entity.get());
			break;
	}

	return new_entity;
}
shared_ptr<CEntity> CScene::CreateAnimEntityFromFile(string file_name, const ColiderType::Type type, glm::vec3 normalized_size, bool instanced, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, const glm::mat4 & parent_matrix)
{
	shared_ptr<CEntity> entity = CreateEntityFromFile(file_name, type, normalized_size, instanced, pos, rot, scale, parent_matrix);
	uint mid = entity->GetModelId();
	GetLoader().m_Models[mid]->m_BoneUpdate = true;
	GetLoader().m_Models[mid]->m_TimeUpdate = true;
	GetLoader().m_Models[mid]->AddCurrentFramePtr(&entity->GetAnimationFrame());
	return entity;
}
void CScene::GetGameObjectsRecursive(CGameObject* game_object)
{
	CheckObjectType(game_object);
	for (const auto& subgame_object : game_object->GetSubGameObjects())
	{
		if (abs(glm::length(subgame_object->GetWorldPosition() - m_Camera->GetPosition())) > m_MaxEntityViewDistane)
			continue;

		if (GetCamera()->CheckFrustrumSphereCulling(subgame_object->GetWorldPosition(), 2.5f * subgame_object->GetMaxNormalizedSize()))
			continue;

		CheckObjectType(subgame_object.get());

		GetGameObjectsRecursive(subgame_object.get());
	}
}
std::list<CWaterTile*> CScene::GetWaterTilesInCameraRange()
{
	return m_WaterTilesInCameraRange;
}
std::list<CEntity*> CScene::GetEntitiesInCameraRange()
{
	return m_EntitiesInCameraRange;
}

void CScene::CheckObjectsInCameraRange()
{
	m_EntitiesInCameraRange.clear();
	m_WaterTilesInCameraRange.clear();

	CheckTerrainInCameraRange();

	for (const auto& game_object : m_GameObjects)
	{
		GetGameObjectsRecursive(game_object.get());
	}
}

void CScene::CheckObjectType(CGameObject * go)
{
	if (GetLoader().m_Models[go->GetModelId()]->m_TimeUpdate)
		GetLoader().AddModelToUpdate(go->GetId(), go->GetModelId());

	CEntity* entity = dynamic_cast<CEntity*>(go);
	if (entity != nullptr)
	{
		m_EntitiesInCameraRange.push_back(entity);
		return;
	}
	CWaterTile* water_tile = dynamic_cast<CWaterTile*>(go);
	if (water_tile != nullptr)
	{
		m_WaterTilesInCameraRange.push_back(water_tile);
		return;
	}
	
}

std::list<CEntity*> CScene::GetPhysicsEntitiesInRange(const glm::vec3& position)
{
	std::list<CEntity*> entities;
	for (const auto& entity : m_PhysicsEntities)
	{
		if (glm::length(entity->GetWorldPosition() - position) < m_PhysicsDistance)
			entities.push_back(entity);
	}
	return entities;
}

CGameObject* CScene::FindGameObject(const uint& id)
{
	CGameObject* go = nullptr;

	for (const auto& game_object : m_GameObjects)
	{
		if (game_object->GetId() == id)
			return game_object.get();
	}
	return go;
}

shared_ptr<CEntity> CScene::FindSubEntity(shared_ptr<CEntity>& entity, int id)
{
	/*shared_ptr<CEntity> founded_entity = nullptr;
	for (auto& entity : entity->GetSubGameObjects())
	{
		if (entity->GetId() == id)
		{
			return entity;
		}
		founded_entity = FindSubEntity(entity, id);
		if (founded_entity != nullptr)
			return founded_entity;
	}*/
	return nullptr;
}

CTerrain * CScene::FindTerrainById(int id)
{
	//for (CTerrain& terrain : m_Terrains)
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount; x++)
		{
			CTerrain& terrain = *GetTerrain(x, y);
			if (terrain.GetId() == id)
			{
				return &terrain;
			}
		}
	return nullptr;
}


CGameObjectsList & CScene::GetGameObjects()
{
	return m_GameObjects;
}

CGameObjectsList& CScene::GetTerrains()
{ 
	return m_Terrains;
}

CTerrain* CScene::GetTerrain(int x, int y)
{
	if (m_TerrainsPtr.empty()) return nullptr;

	if ((x + y*m_TerrainsCount) > m_Terrains.size())
		return nullptr;
	return m_TerrainsPtr[x + y*m_TerrainsCount];
}

const vector<CLight>& CScene::GetLights() const 
{
	return m_Lights;
}

std::list<CTerrain*> CScene::GetTerrainsInCameraRange()
{
	return m_TerrainInCameraRange;
}

void CScene::CheckTerrainInCameraRange()
{
	m_TerrainInCameraRange.clear();
	m_InteractedEntitiesInCameraRange.clear();

	int x_camera, z_camera, view_radius = m_TerrainViewRadius;
	TerrainNumber(m_Camera->GetPositionXZ(), x_camera, z_camera);
	for (int y = z_camera - view_radius; y < z_camera + view_radius + 1; y++)
		for (int x = x_camera - view_radius; x < x_camera + view_radius + 1; x++)
		{
			if (y < 0 || x < 0 || y > m_TerrainsCount || x > m_TerrainsCount)
				continue;
			CTerrain* terrain = GetTerrain(x, y);
			if (terrain == nullptr)
				continue;
			if (!terrain->m_IsInit) continue;

			for (auto& e : terrain->GetSubInteractedObjects())
			{
				m_InteractedEntitiesInCameraRange.push_back(e);
			}			

			//if (m_Camera->CheckFrustrumSphereCulling(terrain->m_WorldCenterPosition, terrain->GetSize() / 1.5f)) continue;
			
			m_TerrainInCameraRange.push_back(terrain);

			for (const auto& game_object : terrain->GetSubGameObjects())
			{
				GetGameObjectsRecursive(game_object.get());
			}
		}

	for (auto& e : m_InteractedObjects)
	{
		m_InteractedEntitiesInCameraRange.push_back(e);
	}
}

const string& CScene::GetName()
{ 
	return m_Name;
}

glm::vec3 CScene::CreatePositionVector(float x, float z, float y_offset) 
{
	return glm::vec3(x, GetHeightOfTerrain(x, z) + y_offset, z);
}

glm::vec3 CScene::CreatePositionVector(glm::vec2 xzPos, float yOffset) 
{
	return CreatePositionVector(xzPos.x, xzPos.y, yOffset);
}

const float CScene::GetHeightOfWorld(float x, float y, float z)
{
	float height = GetHeightOfTerrain(x, z);

	for (const auto& entity : m_PhysicsEntities)
	{
		if (glm::length(entity->GetPositionXZ() - glm::vec2(x, z)) < 0.1f)
			continue;
		float h = entity->m_RigidBody.m_Colider.GetHeight(x, y, z);
		if (h > height)
			height = h;
	}
	return height;
}

//return height form current terrain
const float CScene::GetHeightOfTerrain(float x, float z)
{
	int xx, zz;
	TerrainNumber(x, z, xx, zz);
	if (xx >= 0 && zz >= 0)
	{
		CTerrain* terrain = GetTerrain(xx, zz);
		if(terrain!=nullptr)
			return terrain->GetHeightofTerrain(x, z);
	}		 
	return -1;
}

const float CScene::GetHeightOfTerrain(glm::vec2 xzPos)
{
	return GetHeightOfTerrain(xzPos.x, xzPos.y);
}

void CScene::TerrainNumber(float world_x, float world_z, int & x, int & z)
{
	x = static_cast<unsigned int>( world_x / TERRAIN_SIZE);
	z = static_cast<unsigned int>(world_z / TERRAIN_SIZE);

	if (x < 0 && x > m_TerrainsCount && z < 0 && z > m_TerrainsCount)
	{
		z = -1;
		x = -1;
	}
	else
	{
		CTerrain* terrain = GetTerrain(x, z);
		if (terrain != nullptr)
		if (!terrain->m_IsInit)
		{
			z = -1;
			x = -1;
		}
	}
}

void CScene::TerrainNumber(glm::vec2 xz_pos, int & x, int & z)
{
	TerrainNumber(xz_pos.x, xz_pos.y, x, z);
}


void CScene::SetDirectionalLightColour(const glm::vec3 & color)
{
	m_DirectionalLight.SetColor(color);
}

const CLight & CScene::GetDirectionalLight() const
{
	return m_DirectionalLight;
}

const glm::vec3& CScene::GetDirectionalLightPosition() const
{
	return m_DirectionalLight.GetPosition();
}

glm::vec3 CScene::GetMousePickerTarget()
{
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount; x++)
		{	
		}
		return glm::vec3();
}

void CScene::SetEntityToMousePointByKey(std::shared_ptr<CEntity>& entity)
{
	if (!m_Game->GetInputManager().GetKey(KeyCodes::M)) return;
	for (unsigned int y = 0; y < m_TerrainsCount; y++)
		for (unsigned int x = 0; x < m_TerrainsCount; x++)
	{
		CTerrain* terrain = GetTerrain(x, y);
//		glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition());
	//	entity->SetPosition(CreatePositionVector(point.x, point.z, entity->GetAttachYOffset()));		
	}
}

void CScene::Reset()
{
}

CScene::~CScene()
{
}
void CScene::LoadTerrainsFloora(string file_name)
{
	std::ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open flora: " << file_name << "." << std::endl;
		return;
	}
	std::string line;
	std::string all_file;
	string object;
	std::vector<STransform> transforms;
	glm::vec3 normalized_size(0);
	int x = -1;
	bool creating_object = false;
	while (std::getline(file, line))
	{
		if (line.size() < 2) continue;

		if (line[0] == '#' && line[1] != '#')
		{
			line = line.substr(1);
			transforms.clear();
			cout << "New Terrain Object:\nName: " << line << endl;
			object = line;
			x++;
			creating_object = true;
		}
		else if (line[0] == 'v' && creating_object)
		{
			line = line.substr(1);
			glm::vec2 position =  Get::Vector2d(line);
			Utils::PrintVector("Object position: ", position);
			for(int x= 0; x < 100 ; x++)
				transforms.push_back(STransform(position + 5.f*glm::vec2(rand()%100, rand()%100)));
		}
		else if (line[0] == 'n' && creating_object)
		{
			line = line.substr(1);
			normalized_size = Get::Vector3d(line);
			Utils::PrintVector("Normalized size: ", normalized_size);
		}
		else if (line[0] == '#' && line[1] == '#' && creating_object)
		{
			cout << "End Object"<< endl;
		//	AddInstancedEntityFromFile(object, transforms, normalized_size);
			creating_object = false;
		}
	}
	file.close();

	return;
}