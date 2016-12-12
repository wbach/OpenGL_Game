#include "Entity.h"

CEntity::CEntity()
	: CEntity(glm::vec3(0))
{
}

CEntity::CEntity(glm::vec3 pos)
	: CEntity(pos, glm::vec3(0))
{

}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot)
	: CEntity(pos, rot, glm::vec3(1))
{
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)

{	
	m_Name = "No name entity";
	AddTransform(STransform(pos, rot, scale));
	m_AttachYoffset = 0.f;
}

CEntity::~CEntity() {
	std::cout << "Destructor " << typeid(*this).name() << endl;
}

void CEntity::Update()
{

}



