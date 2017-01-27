#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "OpenGLObject.h"
#include "Material.h"
#include "Face.h"
#include <map>
#include <list>
using namespace std;
namespace VertexBufferObjects
{
	enum Type
	{
		INDICES = 0,
		POSITION,
		TEXT_COORD,
		NORMAL,
		TANGENT,
		TRANSFORM_MATRIX,
		BONES,
		COUNT,
	};
};
namespace LoaderType
{
	enum Type
	{
		ASSIMP = 0,
		FBX,
		NONE,
		COUNT
	};
}
const int NUM_BONES_PER_VEREX = 4;
struct SVertexBoneData
{
	unsigned int ids[NUM_BONES_PER_VEREX];
	float weights[NUM_BONES_PER_VEREX];

	SVertexBoneData()
	{
		memset(ids, 0, sizeof(ids));
		memset(weights, 0, sizeof(weights));
		/*for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++)
		{
			ids[i] = 0;
			weights[i] = 0;
			return;			
		}*/
	}

	void AddBoneData(unsigned int bone_id, float weight)
	{
		for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++) 
		{
			if ( abs(weights[i]) < FLT_MIN)
			{
				ids[i] = bone_id;
				weights[i] = weight;
				return;
			}
		}
		cout << "[Error] To many bones per vertex. Current limit : " << NUM_BONES_PER_VEREX << endl;
	}
};
struct SBoneInfo
{
	glm::mat4 BoneOffset = glm::mat4(0.f);
	glm::mat4 FinalTransformation = glm::mat4(0.f);
};

struct SBonesInfo
{
	vector<SVertexBoneData> bones;
	std::map<string, unsigned int> bone_mapping; // maps a bone name to its index
	unsigned int num_bones = 0;
	vector<SBoneInfo> bone_info;
};

enum LodingPass
{
	NONE = 0,
	FIRST,
	OPENGL,
	DONE
};

class CModel;
class CMesh : public COpenGLObject
{
public:
	CMesh();
	CMesh(SMaterial material);
	CMesh(const CMesh & mesh);
	~CMesh();
	virtual void OpenGLLoadingPass() override;
	virtual void OpenGLPostLoadingPass() override;

	void CalculateBoudnigBox(const vector<float>& positions);
	void UpdateVertexPosition(const vector<float>& vertices) const;	
	void CreateVaoMesh();
	void CreateTransformsVbo(std::vector<glm::mat4>& m);
	void UpdateTransformVbo(std::vector<glm::mat4>& m);
	void CreateBoneVbo(const vector<SVertexBoneData>& bones);
	void CreateFaces();	
	void SetInstancedMatrixes(const vector<glm::mat4>& m);

	const bool UseBoneTransform() const { return m_BonesInShader; }
	const GLuint& GetVao() const;
	const GLuint& GetVbo(VertexBufferObjects::Type type) const;	
	const GLuint& GetVertexCount() const;
	const vector<SFace>& GetFaces() const;
	const SMaterial& GetMaterial() const;	

	const glm::vec3& GetBoundingSize();
	const glm::vec3& GetBoundingMin();
	const glm::vec3& GetBoundingMax();
	const glm::vec3& GetBoundingCenter();

	bool IsReadyToRender() const;
private:
	//Bounding for single meshes in model
	glm::vec3	m_BoundingBoxMin, 
				m_BoundingBoxMax,
				m_BoundingCenter,
				m_BoundingSize;
	
	SMaterial	m_Material;

	GLuint			m_Vao;
	GLuint			m_Vbos[VertexBufferObjects::COUNT];
	vector<SFace>	m_Faces;
	GLuint			m_VertexCount;

	bool m_IsInit = false;
	bool m_TransformVboCreated = false;
	bool m_BonesInShader = false;

	vector<float> positions;
	vector<float> text_coords;
	vector<float> normals;
	vector<float> tangents;
	vector<unsigned short> indices;
	vector<SVertexBoneData> m_Bones;
	vector<glm::mat4> m_InstancedMatrixes;

	friend class CModel;

	int* m_ObjectInstancedCount;

	LodingPass m_LoadingPass;
};
class CModel
{	
public:
	CModel();
	virtual ~CModel() {}
	virtual void InitModel(string file_name) = 0;
	virtual void Update(float dt) {};
	virtual const std::vector<glm::mat4>& GetBonesTransforms(unsigned int mesh_id) {return  std::vector<glm::mat4>();}
	virtual void AddCurrentFramePtr(unsigned int* ptr) { m_CurrentFrames.push_back(ptr); }
	virtual void SetCurrentFrame(unsigned int& current_frame) {}
	virtual void SetTime(const float& time) {}


	void CreateTransformsVbo(std::vector<glm::mat4>& m);

	CMesh* AddMesh(string name, vector<float>& positions, vector<float>& text_coords, vector<float>& normals, vector<float>& tangents,
		vector<unsigned short>& indices, SMaterial& material, vector<SVertexBoneData>& bones = vector<SVertexBoneData>());
	
	void		SetUseInstancedRendering(const bool& s) { m_InstancedRendering = s; }
	const bool&	UseInstacedRendering() const { return m_InstancedRendering; }
	void	CreateMeshesVaos();
	void	CalculateBoudnigBox();

	void	SetName(const std::string& name) { m_Name = name; }
	void	SetId(const unsigned int& id) { m_Id = id; }
	const	string&			GetName() const;
	const	vector<CMesh>&	GetMeshes() const;	
			vector<CMesh>&	GetModifyMeshes();


	float	GetBoundingMaxSize();	
	const unsigned int GetInstancedSize() const;

	glm::mat4 CalculateNormalizedMatrix(float w, float h, float z);
	
	bool m_BoneUpdate;
	bool m_TimeUpdate;
	bool m_UseFakeLight;
	unsigned int m_InstancedSize;
protected:
	string			 m_Name;
	vector<CMesh>	 m_Meshes;
	LoaderType::Type m_LoaderType;	
	
	bool m_InstancedRendering;
	
	//Bounding for all meshes in model
	glm::vec3	m_BoundingBoxMin,
				m_BoundingBoxMax,
				m_BoundingCenter,
				m_BoundingSize;

	//One model in memory for all entities, so we need have time from outside to update (each entity can have diffrent animation progress)
	std::list<unsigned int*> m_CurrentFrames;

	//place in loader
	unsigned int m_Id;
};

