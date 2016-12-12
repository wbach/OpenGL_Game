#include "MousePicker.h"

CMousePicker::CMousePicker() 
: m_Terrains(nullptr) 
{}

CMousePicker::CMousePicker(shared_ptr<CCamera>& cam, glm::vec2 window_size, glm::mat4 projection)
	: m_WindowSize(window_size)
	, m_ProjectionMatrix(projection)
	, m_Camera(cam)
	, RECURSION_COUNT(200)
	, RAY_RANGE(600)
	, m_Terrains(nullptr)
{
}

void CMousePicker::SetTerrainsPtr(std::vector<CTerrain*>* terrains, int height)
{
	m_Terrains = terrains;
	m_TerrainsHeight = height;
}

CTerrain* CMousePicker::GetMousePointOnTerrain(glm::vec3& point, glm::vec2 mous_pose)
{
	m_CurrentRay = CalculateMouseRay(mous_pose);
	if (IntersectionInRange(0, RAY_RANGE, m_CurrentRay))
	{
		return BinarySearch(point, 0, 0, RAY_RANGE, m_CurrentRay);
	}
	return false;
}
glm::vec3 CMousePicker::CalculateMouseRay(glm::vec2 mouse_pos)
{
	glm::vec2  normalized_coords = GetNormalisedDeviceCoordinates(mouse_pos.x, m_WindowSize.y - mouse_pos.y);
	glm::vec4  clipCoords(normalized_coords.x, normalized_coords.y, -1.0f, 1.0f);
	glm::vec4  eyeCoords = ToEyeCoords(clipCoords);
	glm::vec3  worldRay = ToWorldCoords(eyeCoords);
	return worldRay;
}
glm::vec3 CMousePicker::ToWorldCoords(glm::vec4 eyeCoords)
{
	glm::mat4 invertedView = glm::inverse(m_Camera->GetViewMatrix());
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

	return mouseRay;
}
glm::vec4 CMousePicker::ToEyeCoords(glm::vec4 clipCoords)
{
	glm::mat4 invertedProjection = glm::inverse(m_ProjectionMatrix);
	glm::vec4 eyeCoords = invertedProjection*clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}
glm::vec2 CMousePicker::GetNormalisedDeviceCoordinates(float mouseX, float mouseY) {
	float x = (2.0f * mouseX) / m_WindowSize.x - 1.0f;
	float y = (2.0f * mouseY) / m_WindowSize.y - 1.0f;
	return glm::vec2(x, y);
}
glm::vec3 CMousePicker::GetPointOnRay(glm::vec3 ray, float distance)
{
	glm::vec3 cam_pos = m_Camera->GetPosition();
	glm::vec3 scaled_ray(ray.x * distance, ray.y * distance, ray.z * distance);
	return (cam_pos + scaled_ray);
}

CTerrain* CMousePicker::BinarySearch(glm::vec3& out, int count, float start, float finish, glm::vec3 ray) {
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		out = GetPointOnRay(ray, half);
		return GetTerrain(out.x, out.z);
	}
	if (IntersectionInRange(start, half, ray)) {
		return BinarySearch(out, count + 1, start, half, ray);
	}
	else {
		return BinarySearch(out, count + 1, half, finish, ray);
	}
}

bool CMousePicker::IntersectionInRange(float start, float finish, glm::vec3 ray)
{
	glm::vec3 startPoint = GetPointOnRay(ray, start);
	glm::vec3 endPoint = GetPointOnRay(ray, finish);
	if (!IsUnderGround(startPoint) && IsUnderGround(endPoint))
		return true;
	else
		return false;
}

bool CMousePicker::IsUnderGround(glm::vec3 testPoint)
{
	CTerrain* terrain = GetTerrain(testPoint.x, testPoint.z);
	float height = 0;

	if (terrain != nullptr)
	{
		height = terrain->GetHeightofTerrain(testPoint.x, testPoint.z);
	}

	if (testPoint.y <= height)
	{
		return true;
	}
		
	else
		return false;
}
CTerrain* CMousePicker::GetTerrain(float world_x, float world_z)
{
	int x = static_cast<int>(world_x / TERRAIN_SIZE);
	int z = static_cast<int>(world_z / TERRAIN_SIZE);

	int i = x + m_TerrainsHeight*z;

	if ( i > m_Terrains->size() || i < 0)
		return nullptr;
	//return nullptr;
	return (*m_Terrains)[i];
}