#include "Model.h"


CModel::CModel()
	: m_InstancedRendering(false)
	, m_Name("No name model")
	, m_LoaderType(LoaderType::NONE)
	, m_BoundingBoxMin(0)
	, m_BoundingBoxMax(0)
	, m_BoundingCenter(0)
	, m_BoundingSize(0)
{
	m_UseFakeLight = false;
	m_TimeUpdate = false;
	m_BoneUpdate = false;
}

void CModel::CreateTransformsVbo(std::vector<glm::mat4>& m)
{
	for (auto& mesh : m_Meshes)
	{
		mesh.CreateTransformsVbo(m);
	}
	m_InstancedRendering = true;
	m_InstancedSize = m.size();
}

CMesh* CModel::AddMesh(string name, vector<float> &positions, vector<float>&text_coords, vector<float>&normals, vector<float>&tangents,vector<unsigned short> &indices, SMaterial &material, vector<SVertexBoneData>& bones)
{
	CMesh mesh;
	// Normalize to define scale (height) 1 unit = 1 metr
	mesh.CalculateBoudnigBox(positions);
	mesh.CreateFaces();
	m_Name = name;
	mesh.m_Material = material;	
	mesh.m_VertexCount = indices.size();
	mesh.positions = positions;
	mesh.normals = normals;
	mesh.indices = indices;
	mesh.tangents = tangents;
	mesh.text_coords = text_coords;
	mesh.m_Bones = bones;
	m_Meshes.push_back(mesh);
	return &m_Meshes.back();
}
void CModel::CreateMeshesVaos()
{
	for (auto& mesh : m_Meshes)
		mesh.CreateVaoMesh();
}
void CModel::CalculateBoudnigBox()
{
	bool first = true;
	for (const auto& mesh : m_Meshes)
	{
		if (first)
		{
			m_BoundingBoxMin = mesh.m_BoundingBoxMin;
			m_BoundingBoxMax = mesh.m_BoundingBoxMax;
			first = false;
		}
		else
		{
			m_BoundingBoxMin = Utils::CalculateMinimumVector(m_BoundingBoxMin, mesh.m_BoundingBoxMin);
			m_BoundingBoxMax = Utils::CalculateMinimumVector(mesh.m_BoundingBoxMax, m_BoundingBoxMax);
			m_BoundingBoxMax = Utils::CalculateMinimumVector(m_BoundingBoxMax, mesh.m_BoundingBoxMax);
		}
	}
	m_BoundingSize = m_BoundingBoxMax - m_BoundingBoxMin;
	m_BoundingCenter = (m_BoundingBoxMax + m_BoundingBoxMin) / 2.f;
}
glm::mat4 CModel::CalculateNormalizedMatrix(float w, float h, float z)
{
	int axis;
	if (w == 0 && h == 0 && z == 0)
		return glm::mat4(1);
	else if (w != 0 && h == 0 && z == 0)
		axis = 1;
	else if (w == 0 && h != 0 && z == 0)
		axis = 2;
	else if (w == 0 && h == 0 && z != 0)
		axis = 3;

	CalculateBoudnigBox();

	glm::vec3 scale_vector;
	switch (axis)
	{
	case 1:
		scale_vector = glm::vec3(w / m_BoundingSize.x, w / m_BoundingSize.x, w / m_BoundingSize.x);
		break;
	case 2: scale_vector = glm::vec3(h / m_BoundingSize.y, h / m_BoundingSize.y, h / m_BoundingSize.y);
		break;
	case 3: scale_vector = glm::vec3(z / m_BoundingSize.z, z / m_BoundingSize.z, z / m_BoundingSize.z);
		break;
	default: scale_vector = glm::vec3(w / m_BoundingSize.x, h / m_BoundingSize.y, z / m_BoundingSize.z);
		break;
	}
	glm::mat4 normalized_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), scale_vector);	
	return normalized_matrix;
}
const string & CModel::GetName() const
{
	return m_Name;
}
vector<CMesh>& CModel::GetModifyMeshes()
{
	return m_Meshes;
}
const vector<CMesh>& CModel::GetMeshes() const
{
	return m_Meshes;
}
float CModel::GetBoundingMaxSize()
{
	float max = FLT_MIN;
	for (auto& mesh : m_Meshes)
	{
		if (max < mesh.GetBoundingSize().x)
			max = mesh.GetBoundingSize().x;
		if (max < mesh.GetBoundingSize().y)
			max = mesh.GetBoundingSize().y;
		if (max < mesh.GetBoundingSize().z)
			max = mesh.GetBoundingSize().z;
	}

	return max;
}
const unsigned int CModel::GetInstancedSize() const
{
	return m_InstancedSize;
}
CMesh::CMesh() 
	: m_IsInit(false)
	, m_LoadingPass(LodingPass::NONE)
{
	m_ObjectInstancedCount = new int(1);
}
CMesh::CMesh(SMaterial material)
: m_Material(material)
, m_IsInit(false)
, m_LoadingPass(LodingPass::NONE)
{
	m_ObjectInstancedCount = new int(1);
}
CMesh::CMesh(const CMesh & mesh)
	: m_Vao(mesh.m_Vao)
	, m_BonesInShader(mesh.m_BonesInShader)
	, m_TransformVboCreated(mesh.m_TransformVboCreated)
	, m_IsInit(mesh.m_IsInit)
	, m_Faces(mesh.m_Faces)
	, m_VertexCount(mesh.m_VertexCount)
	, m_Material(mesh.m_Material)
	, positions(mesh.positions)
	, text_coords(mesh.text_coords)
	, normals(mesh.normals)
	, tangents(mesh.tangents)
	, indices(mesh.indices)
	, m_Bones(mesh.m_Bones)
	, m_BoundingBoxMin(mesh.m_BoundingBoxMin)
	, m_BoundingBoxMax(mesh.m_BoundingBoxMax)
	, m_BoundingCenter(mesh.m_BoundingCenter)
	, m_BoundingSize(mesh.m_BoundingSize)
	, m_ObjectInstancedCount (mesh.m_ObjectInstancedCount)
{

	for (int x = 0; x < VertexBufferObjects::COUNT; x++)
	{
		m_Vbos[x] = mesh.m_Vbos[x];
	}
	(*m_ObjectInstancedCount)++;
}
CMesh::~CMesh()
{
	(*m_ObjectInstancedCount)--;

	if ((*m_ObjectInstancedCount) >= 1)
		return;

	delete m_ObjectInstancedCount;
	if (!m_IsInit) return;

	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::INDICES]);
	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::POSITION]);
	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::TEXT_COORD]);
	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::NORMAL]);
	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::TANGENT]);
	if (m_TransformVboCreated)
		glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	if (m_BonesInShader)
		glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::BONES]);
	//glDeleteBuffers(VertexBufferObjects::COUNT, m_Vbos);
	glDeleteVertexArrays(1, &m_Vao);
	m_IsInit = false;
}
void CMesh::CalculateBoudnigBox(const vector<float>& positions )
{
	Utils::CalculateBoudnigBox(positions, m_BoundingBoxMin, m_BoundingBoxMax, m_BoundingSize, m_BoundingCenter);
}

void CMesh::UpdateVertexPosition(const vector<float>& vertices) const
{	
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::POSITION]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
}

void CMesh::CreateVaoMesh()
{
	m_Vao = Utils::CreateVao();
	GLuint vboId = Utils::BindIndicesBuffer(indices); m_Vbos[VertexBufferObjects::INDICES] = vboId;
	if (positions.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(0, 3, positions);
		m_Vbos[VertexBufferObjects::POSITION] = vboId;
	}
	if (text_coords.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(1, 2, text_coords);
		m_Vbos[VertexBufferObjects::TEXT_COORD] = vboId;
	}
	if (normals.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(2, 3, normals);
		m_Vbos[VertexBufferObjects::NORMAL] = vboId;
	}
	if (tangents.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(3, 3, tangents);
		m_Vbos[VertexBufferObjects::TANGENT] = vboId;
	}
	Utils::UnbindVao();
	m_IsInit = true;
}

void CMesh::CreateTransformsVbo(std::vector<glm::mat4>& m)
{
	glBindVertexArray(m_Vao);
	glGenBuffers(1, &m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);	
	for (unsigned int i = 0; i < 4; i++) 
	{	
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(4 + i, 1);			
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m.size(), &m[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_TransformVboCreated = true;
}

void CMesh::UpdateTransformVbo(std::vector<glm::mat4>& m)
{
	glDeleteBuffers(1, &m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	glBindVertexArray(m_Vao);	
	glGenBuffers(1, &m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(4 + i, 1);
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m.size(), &m[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CMesh::CreateBoneVbo(const vector<SVertexBoneData>& bones)
{
	glBindVertexArray(m_Vao);
	glGenBuffers(1, &m_Vbos[VertexBufferObjects::BONES]);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::BONES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(8);
	glVertexAttribIPointer(8, 4, GL_INT, sizeof(SVertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(SVertexBoneData), (const GLvoid*) 16);
	glBindVertexArray(0);	
	m_BonesInShader = true;
}

void CMesh::CreateFaces()
{
	
}

void CMesh::SetInstancedMatrixes(const vector<glm::mat4>& m)
{
	m_InstancedMatrixes = m;
}

void CMesh::OpenGLLoadingPass()
{
	CreateVaoMesh();

	if (!m_Bones.empty())
	{		
		CreateBoneVbo(m_Bones);
	}
	m_LoadingPass = DONE;
}

void CMesh::OpenGLPostLoadingPass()
{
	CreateTransformsVbo(m_InstancedMatrixes);
}

const GLuint& CMesh::GetVao() const
{
	return m_Vao;
}

const GLuint & CMesh::GetVbo(VertexBufferObjects::Type type) const
{
	return m_Vbos[type];
}

const vector<SFace>& CMesh::GetFaces() const
{
	return m_Faces;
}

const SMaterial & CMesh::GetMaterial() const
{
	return m_Material;
}

const glm::vec3 & CMesh::GetBoundingSize()
{
	return m_BoundingSize;
}

const glm::vec3 & CMesh::GetBoundingMin()
{
	return m_BoundingBoxMin;
}

const glm::vec3 & CMesh::GetBoundingMax()
{
	return m_BoundingBoxMax;
}

const glm::vec3 & CMesh::GetBoundingCenter()
{
	return m_BoundingCenter;
}

bool CMesh::IsReadyToRender() const
{
	if (m_LoadingPass == LodingPass::DONE)
		return true;
	return false;
}

const GLuint& CMesh::GetVertexCount() const
{
	return m_VertexCount;
}
