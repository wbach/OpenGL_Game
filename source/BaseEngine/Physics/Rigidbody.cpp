#include "Rigidbody.h"
#include "../Utils/Utils.h"
CRigidbody::CRigidbody()
{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}

CRigidbody::CRigidbody(SSphere sphere)
	: m_Colider(sphere)

{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}
CRigidbody::CRigidbody(SAabb aabb)
	: m_Colider(aabb)

{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}

CRigidbody::CRigidbody(std::vector<SFace>& faces)
	: m_Colider(faces)
{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}

std::list<SCollisionInfo> CRigidbody::CheckCollsions(std::list<CRigidbody>& bodys, unsigned int& owner) const
{
	unsigned int id = -1;
	std::list<SCollisionInfo> list;
	for (CRigidbody& body : bodys)
	{
		id++;
		if (id == owner)
			continue;

		std::list<SCollisionInfo> col_info;
		if (m_Colider.DetectCollision(body.m_Colider, col_info))
		{
			for(SCollisionInfo& info : col_info)
			if (info.collision)
			{
				list.push_back(info);
			}
		}
		
	}
	return list;
}

void CRigidbody::ApplyGravity(float delta_time, float gravity)
{
	if (m_IsGrounded)
		return;	
	m_AngularVelocity.y += gravity * delta_time * m_Mass;
}

void CRigidbody::ReflectVelocity(glm::vec3 normal)
{
	glm::vec3  reflected = glm::reflect(m_AngularVelocity, glm::normalize(normal));
	m_AngularVelocity = reflected * m_InvMass;//*m_Restitution;
}

void CRigidbody::SetPosition(glm::vec3 position)
{
	m_Colider.SetPosition(position);
}
