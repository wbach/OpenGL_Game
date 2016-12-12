#include "GameObject.h"
uint CGameObject::s_ID = 0;

CGameObject::CGameObject()
	: m_NormalizedMatrix(1.f)
	, m_RelativeTransformMatrix(1.f)
	, m_IsCullingChildren(true)
	, m_TransformsInVao(false)
	, m_Instanced(false)
	, m_IsSpecial(false)
{
	m_ID = s_ID++;
}

void CGameObject::AddTransform(STransform transform)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms.push_back(transform);
	m_TransformMatrixes.push_back(Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale));
}

void CGameObject::SetPositionX(float x, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].position.x = x;
	CalculateTransformMatrix(index);
}

void CGameObject::SetPositionY(float y, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].position.y = y;
	CalculateTransformMatrix(index);
}

void CGameObject::SetPositionZ(float z, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].position.z = z;
	CalculateTransformMatrix(index);
}
void CGameObject::SetPosition(glm::vec3 position, unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].position = position;
	CalculateTransformMatrix(i);
}

void CGameObject::SetRotationX(float x, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].rotation.x = x;
	CalculateTransformMatrix(index);
}

void CGameObject::SetRotationY(float y, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].rotation.y = y;
	CalculateTransformMatrix(index);
}

void CGameObject::SetRotationZ(float z, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].rotation.z = z;
	CalculateTransformMatrix(index);
}

void CGameObject::SetRotation(glm::vec3 rotation, unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].rotation = rotation;
	CalculateTransformMatrix(i);
}

void CGameObject::SetScaleX(float x, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].scale.x = x;
	CalculateTransformMatrix(index);
}

void CGameObject::SetScaleY(float y, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].scale.y = y;
	CalculateTransformMatrix(index);
}

void CGameObject::SetScaleZ(float z, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	m_Transforms[index].scale.z = z;
	CalculateTransformMatrix(index);
}

void CGameObject::SetScale(glm::vec3 scale, unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].scale = scale;
	CalculateTransformMatrix(i);
}

void CGameObject::SetTransform(STransform transform, unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i] = transform;
	CalculateTransformMatrix(i);
}

glm::vec3& CGameObject::GetReferencedPosition(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].position;
}

glm::vec3& CGameObject::GetReferencedRotation(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].rotation;
}

glm::vec3& CGameObject::GetReferencedScale(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].scale;
}

glm::vec2 CGameObject::GetPositionXZ(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec2_zero;
	return glm::vec2(m_Transforms[i].position.x, m_Transforms[i].position.z);
}

const glm::vec3& CGameObject::GetLocalPosition(unsigned int i) const
{
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].position;
}

const glm::vec3 CGameObject::GetWorldPosition(unsigned int i) const
{
	//return m_Transforms[i].position;
	glm::vec4 pos = m_RelativeTransformMatrix * glm::vec4(m_Transforms[i].position, 1.f);
	return glm::vec3(pos.x, pos.y, pos.z);
}

const glm::vec3& CGameObject::GetRotation(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].rotation;
}

const glm::vec3& CGameObject::GetScale(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_Transforms.size()) return Utils::s_vec3_zero;
	return m_Transforms[i].scale;
}
const STransform& CGameObject::GetTransform(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_TransformMatrixes.size()) return s_DefaultTransform;
	return m_Transforms[i];
}

std::vector<STransform>& CGameObject::GetTransforms()
{ 
	return m_Transforms; 
}

const glm::mat4& CGameObject::GetTransformMatrix(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (i < 0 || i > m_TransformMatrixes.size()) return Utils::s_mat4_one;
	return m_TransformMatrixes[i];
}

const glm::mat4 & CGameObject::GetRelativeTransformMatrix(unsigned int i)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	return m_RelativeTransformMatrix;
}
void CGameObject::IncrasePosition(float dx, float dy, float dz, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	this->m_Transforms[index].position.x += dx;
	this->m_Transforms[index].position.y += dy;
	this->m_Transforms[index].position.z += dz;
	CalculateTransformMatrix(index);
}

void CGameObject::IncrasePosition(glm::vec3 v, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	this->m_Transforms[index].position.x += v.x;
	this->m_Transforms[index].position.y += v.y;
	this->m_Transforms[index].position.z += v.z;
	CalculateTransformMatrix(index);
}

void CGameObject::IncreaseRotation(float dx, float dy, float dz, unsigned int index)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	this->m_Transforms[index].rotation.x += dx;
	this->m_Transforms[index].rotation.y += dy;
	this->m_Transforms[index].rotation.z += dz;

	if(this->m_Transforms[index].rotation.x > 360.f)
		this->m_Transforms[index].rotation.x -= 360.f;
	if (this->m_Transforms[index].rotation.y > 360.f)
		this->m_Transforms[index].rotation.y -= 360.f;
	if (this->m_Transforms[index].rotation.z > 360.f)
		this->m_Transforms[index].rotation.z -= 360.f;

	if (this->m_Transforms[index].rotation.x < 0.f)
		this->m_Transforms[index].rotation.x += 360.f;
	if (this->m_Transforms[index].rotation.y < 0.f)
		this->m_Transforms[index].rotation.y += 360.f;
	if (this->m_Transforms[index].rotation.z < 0.f)
		this->m_Transforms[index].rotation.z += 360.f;
	CalculateTransformMatrix(index);
}

void CGameObject::CalculateTransformMatrix(unsigned int x)
{
	std::lock_guard<std::mutex>::lock_guard(m_Mutex);
	if (x < 0 || x > m_Transforms.size())
	{
		int i = 0;
		for (STransform transform : m_Transforms)
			m_TransformMatrixes[i++] = Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale) * m_NormalizedMatrix;
	}
	else
	{
		m_TransformMatrixes[x] = Utils::CreateTransformationMatrix(m_Transforms[x].position, m_Transforms[x].rotation, m_Transforms[x].scale);

		for (std::shared_ptr<CGameObject>& entity : m_SubObjects)
		{
			RecursiveSetRelativeTransformMatrix(entity, m_TransformMatrixes[x]);
		}
		m_TransformMatrixes[x] = m_TransformMatrixes[x] * m_NormalizedMatrix;
	}
	m_TransformsInVao = false;
}

void CGameObject::RecursiveSetRelativeTransformMatrix(std::shared_ptr<CGameObject>& e, const glm::mat4& parent_matrix)
{
	e->SetRelativeMatrix(parent_matrix);

	for (std::shared_ptr<CGameObject>& subentity : e->GetSubGameObjects())
	{
		RecursiveSetRelativeTransformMatrix(subentity, parent_matrix * e->GetTransformMatrix());
	}
}
void CGameObject::SetRelativeMatrix(const glm::mat4& parent_matrix)
{
	m_RelativeTransformMatrix = parent_matrix;
}

const std::string & CGameObject::GetTag() const
{
	return m_Tag;
}

const uint & CGameObject::GetId() const
{
	return m_ID;
}

const float CGameObject::GetMaxNormalizedSize()
{ 
	return Utils::GetMaxFromVector(m_NormalizedSize); 
}

CGameObjectsList& CGameObject::GetSubGameObjects()
{
	return m_SubObjects;
}

CGameObjectsPtrList & CGameObject::GetSubInteractedObjects()
{
	return m_InteractionObjects;
}

const int& CGameObject::GetModelId() const
{
	return m_ModelID;
}
const std::string CGameObject::GetNameWithID() const
{
	std::string name = m_Name + "__id_e" + std::to_string(m_ID);
	return name;
}
const std::string& CGameObject::GetName() const
{
	return m_Name;
}
void CGameObject::SetName(std::string name)
{ 
	m_Name = name; 
}
void CGameObject::AddSubbObject(std::shared_ptr<CGameObject> e, const bool& interacted)
{
	m_SubObjects.push_back(e);

	if (interacted)
		m_InteractionObjects.push_back(m_SubObjects.back().get());
}
void CGameObject::SetModelId(unsigned int model_id, std::string path)
{
	m_ModelID = model_id;
	m_FullPathFile = path;
}
void CGameObject::SetFullPath(std::string path)
{ 
	m_FullPathFile = path; 
}
const std::string& CGameObject::GetFullPath() const
{ 
	return m_FullPathFile; 
}
const glm::mat4& CGameObject::GetNormalizedMatrix()
{
	return m_NormalizedMatrix;
}

void CGameObject::SetNormalizedSize(const glm::vec3 & v)
{
	m_NormalizedSize = v;
}
const glm::vec3 & CGameObject::GetNormalizedSize()
{
	return m_NormalizedSize;
}
void CGameObject::SetNormalizedMatrix(const glm::mat4& m)
{
	m_NormalizedMatrix = m;
	CalculateTransformMatrix();
}
void CGameObject::SetIsCullingChildren(const bool & is)
{
	m_IsCullingChildren = is;
}
const bool& CGameObject::GetIsCullingChildren()
{
	return m_IsCullingChildren;
}