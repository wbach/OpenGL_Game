#pragma once
#include <string>
#include "../Engine/GameObject.h"
#include "../Utils/Utils.h"
#include "../Physics/Rigidbody.h"
#include <vector>
#include <list>
#include <mutex>
#include <memory>

using namespace std;

class CEntity : public CGameObject
{	
public:		
	CEntity();
	CEntity(glm::vec3 pos);
	CEntity(glm::vec3 pos, glm::vec3 rot);
	CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);	
	~CEntity();	

	void Update();
	unsigned int& GetAnimationFrame() { return m_CurrentAnimationFrame; }
	float& GetAttachYOffset() { return m_AttachYoffset; }	

	CRigidbody	m_RigidBody;
protected:	
	float m_AttachYoffset;
	//One model in memory for all entities, so we need have time from outside (each entity can have diffrent animation progress)
	unsigned int m_CurrentAnimationFrame = 0;
};
