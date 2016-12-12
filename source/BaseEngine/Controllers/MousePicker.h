#pragma once
#include "../Camera/Camera.h"
#include "../Terrain/Terrain.h"

class CMousePicker 
{
public:
	CMousePicker();
	CMousePicker(shared_ptr<CCamera>& cam, glm::vec2 window_size, glm::mat4 projection);
	CTerrain* GetMousePointOnTerrain(glm::vec3& point, glm::vec2 mous_pose);
	void SetTerrainsPtr(std::vector<CTerrain*>* terrains, int height);
private:
	glm::vec3 CalculateMouseRay(glm::vec2 mouse_pos);
	glm::vec3 ToWorldCoords(glm::vec4  eyeCoords);
	glm::vec4 ToEyeCoords(glm::vec4 clipCoords);
	glm::vec2 GetNormalisedDeviceCoordinates(float mouseX, float mouseY);

	//**********************************************************

	glm::vec3 GetPointOnRay(glm::vec3 ray, float distance);
	CTerrain* BinarySearch(glm::vec3& out, int count, float start, float finish, glm::vec3 ray);
	bool IntersectionInRange(float start, float finish, glm::vec3 ray);
	bool IsUnderGround(glm::vec3 testPoint);
	CTerrain* GetTerrain(float world_x, float world_z);
	int			RECURSION_COUNT;
	float		RAY_RANGE;
	glm::vec3	m_CurrentRay;

	glm::mat4	m_ProjectionMatrix;
	shared_ptr<CCamera>	m_Camera;

	glm::vec2	m_WindowSize;

	std::vector<CTerrain*>* m_Terrains;
	int m_TerrainsHeight;
};