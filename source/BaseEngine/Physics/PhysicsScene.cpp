#include "PhysicsScene.h"
#include "../Utils/Utils.h"
void CPhysicsScene::Update(float delta_time)
{
	//int x = 0;
	//for (CRigidbody* a : m_Rigibodys)
	//{		
	//	bool IsColl = false;
	//	int y = -1;
	//	for (CRigidbody* b : m_Rigibodys)
	//	{
	//		
	//		y++;
	//		if (y == x) continue;
	//		glm::vec3 normal; float penetration;
	//		SCollisionInfo col_info;
	//	//	std::cout << "Check Collision" << a->m_Colider.m_ColiderType << "| " << b->m_Colider.m_ColiderType << std::endl;

	//	/*	if (a->m_Colider.m_ColiderType == ColiderType::MESH_COLIDER && b->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER)
	//		{
	//			if (a->m_Colider.DetectCollision(b->m_Colider, col_info))
	//			{
	//				b->m_Colider.m_UpdateVector = col_info.updateVector;
	//				IsColl = true;
	//			}
	//		}*/
	//		if (a->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER && b->m_Colider.m_ColiderType == ColiderType::MESH_COLIDER)
	//		{
	//			if (b->m_Colider.DetectCollision(a->m_Colider, col_info))
	//			{
	//				if(!a->m_Static)
	//				a->m_Colider.m_UpdateVector = col_info.updateVector;
	//				a->m_Colider.m_MinYPos = col_info.collisionY;
	//				IsColl = true;
	//			}
	//		}
	//		


	//		continue;
	//		if (a->m_Colider.DetectCollision(b->m_Colider, col_info))
	//		{				
	//			
	//			if (a->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER && b->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER)
	//			{
	//				if (SphereSphere(a->m_Colider, b->m_Colider, normal, penetration))
	//				{						
	//					ResolveCollision(*a, *b, normal, delta_time);
	//					PositionalCorrection(*a, *b, penetration, normal);
	//					IsColl = true;
	//				}
	//			}				
	//		}			
	//		if (a->m_Colider.m_ColiderType == ColiderType::BOX_COLIDER && b->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER)
	//		{
	//			if (AABBvsSphere(a->m_Colider, b->m_Colider, normal, penetration))
	//			{
	//				ResolveCollision(*a, *b, normal, delta_time);
	//				PositionalCorrection(*a, *b, penetration, normal);
	//			}
	//		}
	//		if (a->m_Colider.m_ColiderType == ColiderType::SPHERE_COLIDER && b->m_Colider.m_ColiderType == ColiderType::BOX_COLIDER)
	//		{
	//			if (AABBvsSphere(b->m_Colider, a->m_Colider, normal, penetration))
	//			{
	//				ResolveCollision(*b, *a, normal, delta_time);
	//				PositionalCorrection(*b, *a, penetration, normal);
	//			}
	//		}
	//	}
	//	if(!a->m_Static)
	//		a->ApplyGravity(delta_time, m_Gravity);
	//	
	//	x++;
	//}
}
void CPhysicsScene::AddRigidBody(CRigidbody* rigidbody)
{
	m_Rigibodys.push_back(rigidbody);
}

void CPhysicsScene::ResolveCollision(CRigidbody& a, CRigidbody & b, glm::vec3 normal, float dt)
{
	// Calculate relative velocity
	glm::vec3 rv = b.m_AngularVelocity - a.m_AngularVelocity;

		// Calculate relative velocity in terms of the normal direction
		float velAlongNormal = glm::dot(rv, normal);

		// Do not resolve if velocities are separating
		if (velAlongNormal > 0)
			return;

		// Calculate restitution
		float e = std::min(a.m_Restitution, b.m_Restitution);

		// Calculate impulse scalar
		float j = -(1 + e) * velAlongNormal;
		j /= a.m_InvMass + b.m_InvMass;

		// Apply impulse
		glm::vec3 impulse = j * normal;
		a.m_AngularVelocity -= a.m_InvMass * impulse;
		b.m_AngularVelocity += b.m_InvMass * impulse;
		
		return;
	//// Calculate relative velocity
	//glm::vec3 rv = b.m_AngularVelocity - a.m_AngularVelocity;

	//// Calculate relative velocity in terms of the normal direction
	//float velAlongNormal = glm::dot(rv, normal);
	////std::cout << "velAlongNormal: " << velAlongNormal << std::endl;
	//// Do not resolve if velocities are separating
	//if (velAlongNormal > 0) 	return;


	//// Calculate restitution
	//float e = std::min(a.m_Restitution, b.m_Restitution);

	//// Calculate impulse scalar
	//float j = -(1 + e) * velAlongNormal;
	//j /= 1 / a.m_Mass + 1 / b.m_Mass;
	//
	//glm::vec3 a_vel = a.m_AngularVelocity;
	//glm::vec3 b_vel = b.m_AngularVelocity;
	//glm::vec3 n = glm::normalize(a.m_Colider.m_Sphere.position - b.m_Colider.m_Sphere.position);
	//float s = glm::dot(a_vel - b_vel, n);
	//if ( s > 0)
	//{
	//	
	//	a.m_AngularVelocity = glm::reflect(a_vel - b_vel, n) * (1.f / a.m_Mass + 1 / b.m_Mass);//a.m_Restitution ;
	//}
	//n = glm::normalize(a.m_Colider.m_Sphere.position - b.m_Colider.m_Sphere.position);

	//s = glm::dot(b_vel - a_vel, n);
	//if (s > 0)
	//{
	//	n = -n;
	//	b.m_AngularVelocity = glm::reflect(b_vel - a_vel, n)* (1.f / a.m_Mass + 1 / b.m_Mass);//b.m_Restitution;
	//}
	//// Apply impulse
	//glm::vec3 impulse = j * normal;
	//a.m_AngularVelocity += 1 / a.m_Mass * impulse;
	//b.m_AngularVelocity -= 1 / b.m_Mass * impulse;


}
void CPhysicsScene::PositionalCorrection(CRigidbody& a, CRigidbody& b, float& penetration, glm::vec3 normal)
{
	const float percent = 0.8f; // usually 20% to 80%
	const float slop = 0.01f; // usually 0.01 to 0.1
	glm::vec3 correction = std::max(penetration*-1.f - slop, 0.0f) / (a.m_InvMass + b.m_InvMass) * percent * normal;
	a.m_Colider.m_Sphere.position -= a.m_InvMass * correction;
	b.m_Colider.m_Sphere.position += b.m_InvMass * correction;
}
bool CPhysicsScene::AABBvsSphere(CColider & a, CColider & b, glm::vec3 & normal, float & penetration)
{
		// Vector from A to B
		glm::vec3 n = b.GetPosition() - a.GetPosition();

		// Closest point on A to center of B
		glm::vec3 closest = n;

		// Calculate half extents along each axis
		float x_extent = (a.m_Aabb.max.x - a.m_Aabb.min.x) / 2.f;
		float y_extent = (a.m_Aabb.max.y - a.m_Aabb.min.y) / 2.f;
		float z_extent = (a.m_Aabb.max.z - a.m_Aabb.min.z) / 2.f;

		// Clamp point to edges of the AABB
		closest.x = glm::clamp(-x_extent, x_extent, closest.x);
		closest.y = glm::clamp(-y_extent, y_extent, closest.y);
		closest.z = glm::clamp(-z_extent, z_extent, closest.z);

		bool inside = false;

		// Circle is inside the AABB, so we need to clamp the circle's center
		// to the closest edge
		if (n == closest)
		{
			inside = true;

				// Find closest axis
				if (abs(n.x) > abs(n.y) && abs(n.x) > abs(n.z))
				{
					// Clamp to closest extent
					if (closest.x > 0)
						closest.x = x_extent;
					else
						closest.x = -x_extent;
				}

			// y axis is shorter
				else if (abs(n.y) > abs(n.z))
				{
					// Clamp to closest extent
					if (closest.y > 0)
						closest.y = y_extent;
					else
						closest.y = -y_extent;
				}
				// y axis is shorter
				else
				{
					// Clamp to closest extent
					if (closest.y > 0)
						closest.z = z_extent;
					else
						closest.z = -z_extent;
				}
		}

		normal = n - closest;
		float d = glm::length2(normal);
		float r = b.m_Sphere.radius;

		// Early out of the radius is shorter than distance to closest point and
		// Circle not inside the AABB
		if (d > r * r && !inside)
			return false;

			// Avoided sqrt until we needed
		d	 = sqrt(d);

			// Collision normal needs to be flipped to point outside if circle was
			// inside the AABB
			if (inside)
			{
				normal = -n;
				penetration = r - d;
			}
			else
			{
				normal = n;
				penetration = r - d;
			}

			return true;
}
bool CPhysicsScene::SphereSphere(CColider& a,CColider& b, glm::vec3& normal, float& penetration) 
{

	// Vector from A to B
	glm::vec3 n = b.m_Sphere.position - a.m_Sphere.position;

	//normal = n;
	//return true;

	float r = a.m_Sphere.radius + b.m_Sphere.radius;
	r *= r;

	if (glm::length2(n) > r)
		return false;

	// Circles have collided, now compute manifold
	float d = glm::length(n); // perform actual sqrt
								 // If distance between circles is not zero
	if (d != 0)
	{
		// Distance is difference between radius and distance
		penetration = r - d;

		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector
		normal = n / d;
		return true;
	}

	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		penetration = a.m_Sphere.radius;
		normal = glm::vec3(1, 0, 0);
		return true;
	}
}