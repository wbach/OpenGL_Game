#pragma once
#include "glm/glm.hpp"
struct SCollisionInfo
{
	glm::vec3 normal = glm::vec3(0.f);
	bool	  collision = false;
	glm::vec3 updateVector = glm::vec3(0.f);
	float	  collisionY = 0.f;
	float	  t = 0.f;
};
struct SFaceVertex
{
	glm::vec3 v1 = glm::vec3(0.f);
	glm::vec3 v2 = glm::vec3(0.f);
	glm::vec3 v3 = glm::vec3(0.f);
};
struct SFace
{
	SFaceVertex vertexes;
	glm::vec3 normal = glm::vec3(0.f);
	glm::vec3 center = glm::vec3(0.f);
	float skipDistance = 0.f;

	void CalculateNormal()
	{
		float Vector1[3];
		float Vector2[3];

		// wyznaczanie wektorow....
		Vector1[0] = vertexes.v1.x - vertexes.v2.x;
		Vector1[1] = vertexes.v1.y - vertexes.v2.y;
		Vector1[2] = vertexes.v1.z - vertexes.v2.z;

		Vector2[0] = vertexes.v2.x - vertexes.v3.x;
		Vector2[1] = vertexes.v2.y - vertexes.v3.y;
		Vector2[2] = vertexes.v2.z - vertexes.v3.z;

		//wyznaczanie normalnych....
		normal.x = Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1];
		normal.y = Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2];
		normal.z = Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0];

		normal = glm::normalize(normal);
	}

};