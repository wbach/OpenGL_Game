#pragma once
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "TextureLoader.h"



class CAssimModel  : public CModel
{
public:
	CAssimModel(CTextureLoader& texture_lodaer);
	virtual ~CAssimModel() override;
	virtual void	InitModel(string file_name) override;
	static void		ReadCollisions(string file_name, vector<float>& postions, vector<float>& normals, vector<unsigned int>& indices);	
	const string&	GetName() const;
private:
	static void RecursiveProcess(const aiScene *scene, aiNode *node, vector<float>& postions, vector<float>& normals, vector<unsigned int>& indices);
	void ProcessMesh(string file_path, aiMesh* mesh, const aiScene* scene);
	void RecursiveProcess(string file_path, aiNode *node, const aiScene *scene);
	static glm::mat4 ToGlmMatrix(const aiMatrix4x4& assimp_atrix);
	static glm::mat4 ToGlmMatrix(const aiMatrix3x3& assimp_atrix);
	virtual void SetTime(const float& time) override;
	void Update(float delta_time) override;
	virtual const std::vector<glm::mat4>& GetBonesTransforms(unsigned int mesh_id) override;

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
	{
		for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) 
		{
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (string(pNodeAnim->mNodeName.data) == NodeName) 
			{
				return pNodeAnim;
			}
		}
		return nullptr;
	}
	unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
		{
			if (AnimationTime < (float) pNodeAnim->mPositionKeys[i + 1].mTime) 
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);
		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}

		assert(0);
		return 0;
	}
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
		{
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}

	void BoneTransform(unsigned int num_bones, vector<SBoneInfo>& bone_info, std::map<string, unsigned int>& bone_mapping, float TimeInSeconds, vector<glm::mat4>& Transforms);
	void ReadNodeHeirarchy(std::map<string, unsigned int>& bone_mapping, vector<SBoneInfo>& bone_info, float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	float m_CurrentTime;
	// For all subMeshes
	std::vector<SBonesInfo> m_BonesInfo;

	string			m_Name;	
	CTextureLoader&	m_TextureLodaer;

	Assimp::Importer importer;
	const aiScene *m_Scene;
	glm::mat4 m_GlobalInverseTransform;

	std::vector<glm::mat4> m_BoneTransformMatrixes[50];

	bool m_IsInit;
};