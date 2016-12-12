#pragma once
#include "Rigidbody.h"
#include "glm/gtx/norm.hpp"
#include <vector>
#include <algorithm>
class CPhysicsScene
{
public:
	void Update(float delta_time);
	void AddRigidBody(CRigidbody* rigidbody);
	std::vector<CRigidbody*> m_Rigibodys;
private:
	bool SphereSphere(CColider& a, CColider & b, glm::vec3& normal, float& penetration);
	void ResolveCollision(CRigidbody& a, CRigidbody& b, glm::vec3 normal, float dt);
	void PositionalCorrection(CRigidbody& a, CRigidbody& b, float& penetration, glm::vec3 normal);
	bool AABBvsSphere(CColider& a, CColider & b, glm::vec3& normal, float& penetration);
	float m_Gravity = -9.8f;
};