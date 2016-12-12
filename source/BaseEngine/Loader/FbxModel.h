#pragma once
#include <string>
#include "Model.h"
#include <GL/glew.h>
#include <fbxsdk.h>
#include "TextureLoader.h"
#include <unordered_map>
#include <thread>
class CFbxModel : public CModel
{
public:
	CFbxModel(CTextureLoader& texture_lodaer);
	~CFbxModel();
	virtual void	InitModel(string file_name) override;
	const	string&	GetName() const;
private:
	void InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene);
	bool LoadFile();
	void LoadCacheRecursive(FbxScene* scene, FbxAnimLayer* anim_layer);
	void LoadCacheRecursive(FbxNode* node, FbxAnimLayer* anim_layer);
	void UnloadCacheRecursive(FbxNode* node);
	void UnloadCacheRecursive(FbxScene* scene);
	void DestroySdkObjects(FbxManager* manager, bool exit_status);
	
	bool InitializeMesh(FbxMesh* mesh);
	bool InitializeMaterial(const FbxSurfaceMaterial* material);

	bool SetCurrentAnimStack(int index);

	void Update(float delta_time) override;	
	void Update();

	void UpdateNodeRecursive(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer, FbxAMatrix& parent_global_position, FbxPose* pose);
	void UpdateNode(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer, FbxAMatrix& parent_global_position,	FbxAMatrix& global_position, FbxPose* pose);
	void UpdateMesh(FbxNode* node, FbxTime& time, FbxAnimLayer* anim_layer, FbxAMatrix& global_position, FbxPose* pose);
	void ReadVertexCacheData(FbxMesh* mesh, FbxTime& time, FbxVector4* verttex_array);
	
	void ComputeShapeDeformation(FbxMesh* mesh, FbxTime& time, FbxAnimLayer* anim_layer, FbxVector4* vertex_array);
	void ComputeSkinDeformation(FbxAMatrix& global_position, FbxMesh* mesh, FbxTime& time, FbxVector4* vertex_vrray, FbxPose* pose);
	void ComputeLinearDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose);
	void ComputeDualQuaternionDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxTime& pTime, FbxVector4* pVertexArray, FbxPose* pPose);
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose);
	
	FbxAMatrix GetGlobalPosition(FbxNode* node, const FbxTime& time, FbxPose* pose, FbxAMatrix* parent_global_position = nullptr);
	FbxAMatrix GetPoseMatrix(FbxPose* pose, int node_index);
	FbxAMatrix GetGeometry(FbxNode* node);

	void MatrixScale(FbxAMatrix& pMatrix, double pValue);
	void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue);
	void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);

	virtual void SetCurrentFrame(unsigned int& current_frame) override
	{
		m_CurrentTime = m_Start;
		m_CurrentFrameBones = current_frame;

		for (unsigned int i = 0; i < current_frame; i++)
		{
			m_CurrentTime += m_FrameTime;
		}
	}
	void OnTimerClick();

	virtual const std::vector<glm::mat4>& GetBonesTransforms(unsigned int mesh_id) override
	{ 
		if(!m_SaveBonesTransform)
			return  m_BoneTransformMatrixes; 

		FbxTime durration = m_Stop - m_Start;
		double a = durration.GetSecondDouble();
		double b = m_CurrentTime.GetSecondDouble();
	
		m_BoneTransformMatrixesPerTime.clear();

		unsigned int startui = m_FrameBonesTransforms * mesh_id;
		for (unsigned int i = startui; i < startui + m_FrameBonesTransforms; i++)
		{
			m_BoneTransformMatrixesPerTime.push_back(m_BoneTransformMatrixes[i]);
		}
		return m_BoneTransformMatrixesPerTime;
	}
	glm::mat4 FbxMatrixToGlm(FbxAMatrix&  m);

	FbxDouble3 GetMaterialProperty(	const FbxSurfaceMaterial* material, const char* property_name, const char* factor_property_name, int*& texture_id);

	std::string		m_FileName;
	std::string		m_FilePath;

	std::vector<SMaterial> m_Materials;

	int				m_PoseIndex;

	FbxManager*		m_SdkManager;
	FbxScene*		m_Scene;
	FbxImporter*	m_Importer;
	FbxAnimLayer*	m_CurrentAnimLayer;
	FbxNode*		m_SelectedNode;

	FbxArray<FbxString*> m_AnimStackNameArray;

	float	m_Time, m_FramePerSecond;
	mutable FbxTime m_FrameTime, m_Start, m_Stop, m_CurrentTime;
	mutable FbxTime m_CacheStart, m_CacheStop;

	string			m_Name;
	CTextureLoader&	m_TextureLodaer;

	std::vector<float> m_UpdateVertices;

	// For all subMeshes
	std::vector<SBonesInfo> m_BonesInfo;
	std::vector<glm::mat4> m_BoneTransformMatrixes;
	std::vector<glm::mat4> m_BoneTransformMatrixesPerTime;

	bool m_SaveBonesTransform = true;
	bool m_SavingBonesTransforms = false;
	unsigned int  m_FrameBonesTransformsOffset = 0;
	unsigned int  m_FrameBonesTransforms = 0;
	unsigned int  m_CurrentFrameBones = 0;
};