#pragma once
#include "../Utils/Types.h"
#include "../Utils/Utils.h"
#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include "glm/glm.hpp"
#include "Transform.h"
class CGameObject;

typedef std::list<std::shared_ptr<CGameObject>> CGameObjectsList;
typedef std::list<CGameObject*> CGameObjectsPtrList;

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject() {}
	virtual void Create() {}
	void SetModelId(unsigned int model_id, std::string path = "");
	void SetName(std::string name);
	void SetFullPath(std::string path);	

	void AddSubbObject(std::shared_ptr<CGameObject> e, const bool& interacted = false);

	void AddTransform(STransform transform);
	void SetTransform(STransform transform, unsigned int i = 0);
	void CalculateTransformMatrix(unsigned int x = 0);
	void RecursiveSetRelativeTransformMatrix(std::shared_ptr<CGameObject>& e, const glm::mat4& parent_matrix);
	void SetRelativeMatrix(const glm::mat4& parent_matrix);
	void SetNormalizedMatrix(const glm::mat4& m);
	void SetNormalizedSize(const glm::vec3& v);

	void SetPositionX(float x, unsigned int index = 0);
	void SetPositionY(float y, unsigned int index = 0);
	void SetPositionZ(float z, unsigned int index = 0);
	void SetPosition(glm::vec3 position, unsigned int i = 0);

	void SetRotationX(float x, unsigned int index = 0);
	void SetRotationY(float y, unsigned int index = 0);
	void SetRotationZ(float z, unsigned int index = 0);
	void SetRotation(glm::vec3 rotation, unsigned int i = 0);

	void SetScaleX(float x, unsigned int index = 0);
	void SetScaleY(float y, unsigned int index = 0);
	void SetScaleZ(float z, unsigned int index = 0);
	void SetScale(glm::vec3 scale, unsigned int i = 0);	

	void IncrasePosition(float dx, float dy, float dz, unsigned int index = 0);
	void IncrasePosition(glm::vec3 v, unsigned int index = 0);
	void IncreaseRotation(float dx, float dy, float dz, unsigned int index = 0);	
	
	void SetBoundingSize(const float& size) { m_BoundingSize = size; }

	const glm::vec3& GetRotation(unsigned int i = 0);
	const glm::vec3& GetScale(unsigned int i = 0);	
	const glm::vec3& GetLocalPosition(unsigned int i = 0) const;
	const glm::vec3  GetWorldPosition(unsigned int i = 0) const;
	glm::vec2		 GetPositionXZ(unsigned int i = 0);
	glm::vec3&		 GetReferencedPosition(unsigned int i = 0);
	glm::vec3&		 GetReferencedRotation(unsigned int i = 0);
	glm::vec3&		 GetReferencedScale(unsigned int i = 0);

	const STransform&		GetTransform(unsigned int i = 0);
	std::vector<STransform>& GetTransforms();
	const glm::mat4&		GetTransformMatrix(unsigned int i = 0);
	std::vector<glm::mat4>&	GetTransformMatrixes() { return m_TransformMatrixes; }
	const glm::mat4&		GetRelativeTransformMatrix(unsigned int i = 0);	
	const glm::mat4&	GetNormalizedMatrix();
	const glm::vec3&	GetNormalizedSize();
	const float			GetMaxNormalizedSize();
	const bool&			IsInsanced() { return m_Instanced; };
	const bool&			IsSpecial() { return m_IsSpecial; };

	CGameObjectsList&		GetSubGameObjects();
	CGameObjectsPtrList&	GetSubInteractedObjects();

	const int&			GetModelId() const;
	const std::string&  GetTag() const;
	const uint&			GetId() const;
	const std::string	GetNameWithID() const;
	const std::string&	GetName() const;
	const std::string&	GetFullPath() const;
	float GetMaxBoundingSize() { return m_BoundingSize; }
	

	void SetIsCullingChildren(const bool& is);
	const bool& GetIsCullingChildren();
protected:
	std::string m_Tag;
	std::string m_FullPathFile;
	std::string m_Name;

	CGameObjectsList	m_SubObjects;
	CGameObjectsPtrList m_InteractionObjects;

	std::mutex	m_Mutex;
	glm::vec3	m_NormalizedSize;
	glm::mat4	m_NormalizedMatrix;
	glm::mat4	m_RelativeTransformMatrix;
	std::vector<glm::mat4>	m_TransformMatrixes;
	std::vector<STransform>	m_Transforms;

	static uint s_ID;
	uint m_ID;

	uint m_ModelID = 0;

	float m_BoundingSize;
	bool m_IsCullingChildren;
	bool m_Instanced; // Not culling using primary position	
	//m_IsSpecial - enity created in code not from file
	bool m_IsSpecial;
	bool m_TransformsInVao;
};