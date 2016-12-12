#pragma once
#include "../Collision/Colider.h"
#include <math.h>
#include <algorithm>
#include <list>

class CRigidbody
{
public:	
	CRigidbody();
	CRigidbody(SSphere sphere);
	CRigidbody(SAabb sphere);
	CRigidbody(std::vector<SFace>& faces);

	std::list<SCollisionInfo> CheckCollsions(std::list<CRigidbody>& bodys, unsigned int& owner) const;

	CColider m_Colider;

	void ApplyGravity(float delta_time, float gravity);
	void ReflectVelocity(glm::vec3 normal);
	void SetPosition(glm::vec3 position);

	glm::vec3 m_AngularVelocity = glm::vec3(0);
	glm::vec3 m_CenterOfMass = glm::vec3(0);

	float m_Mass = 10.f;
	float m_InvMass;
	bool IsUseGravity = true;
	bool m_IsGrounded = false;

	float m_Restitution = 0.7f;
	float m_Magnitude = 0.f;
	bool  m_Static = true;

	std::list<SCollisionInfo> m_IncomeColisonInfo;
};