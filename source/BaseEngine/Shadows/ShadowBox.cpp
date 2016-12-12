#include "ShadowBox.h"

CShadowBox::CShadowBox()
: m_Camera(nullptr)
, m_LightViewMatrix(1.f)
{
}

void CShadowBox::Init(CCamera* camera, glm::vec2 window_size, float fov, float near_plane, float shadows_distance)
{
	m_Offset = 25;
	m_Up = glm::vec4(0, 1, 0, 0);
	m_Forward = glm::vec4(0, 0, -1, 0);
	m_ShadowDistance = shadows_distance;
	m_Camera = camera;
	m_NearPlane = near_plane;
	m_Fov = fov;
	m_WindowSize = window_size;
	CalculateWidthsAndHeights();
}

void CShadowBox::CalculateWidthsAndHeights()
{
	m_FarWidth   = (float)(m_ShadowDistance * tan(Utils::ToRadians(m_Fov)));
	m_NearWidth  = (float)(m_NearPlane * tan(Utils::ToRadians(m_Fov)));
	m_FarHeight  = m_FarWidth / GetAspectRatio();
	m_NearHeight = m_NearWidth / GetAspectRatio();
}

const float CShadowBox::GetAspectRatio() const
{
	return m_WindowSize.x / m_WindowSize.y;
}

void CShadowBox::SetLightViewMatrix(const glm::mat4& matrix)
{
	m_LightViewMatrix = matrix;
}

glm::mat4 CShadowBox::CalculateCameraRotationMatrix() const
{
	if (m_Camera == nullptr)
	{
		std::cout << "[Error] Camera not attach to CShadowBox." << std::endl;
		return glm::mat4();
	}
	glm::mat4 rotation = glm::mat4(1.f);
	rotation *= glm::rotate((float)(-m_Camera->GetYaw()), glm::vec3(0.f, 1.f, 0.f));
	rotation *= glm::rotate((float)(-m_Camera->GetPitch()), glm::vec3(1.f, 0.f, 0.f));
	return rotation;
}

void CShadowBox::Update()
{
	glm::mat4 rotation = CalculateCameraRotationMatrix();
	glm::vec3 forward_vector(rotation * m_Forward);
	glm::vec3 to_far(forward_vector);
	to_far *= m_ShadowDistance;

	glm::vec3 to_near(forward_vector);
	to_near *= m_NearPlane;

	glm::vec3 center_near = to_near + m_Camera->GetPosition();
	glm::vec3 center_far  = to_far  + m_Camera->GetPosition();

	std::vector<glm::vec4> points = CalculateFrustumVertices(rotation, forward_vector, center_near, center_far);

	bool first = true;
	for (const glm::vec4& point : points)
	{
		if (first)
		{
			m_Min.x = point.x;
			m_Max.x = point.x;
			m_Min.y = point.y;
			m_Max.y = point.y;
			m_Min.z = point.z;
			m_Max.z = point.z;
			first = false;
			continue;
		}

		if (point.x > m_Max.x)
		{
			m_Max.x = point.x;
		}
		else if (point.x < m_Min.x)
		{
			m_Min.x = point.x;
		}
		if (point.y > m_Max.y)
		{
			m_Max.y = point.y;
		}
		else if (point.y < m_Min.y)
		{
			m_Min.y = point.y;
		}
		if (point.z > m_Max.z)
		{
			m_Max.z = point.z;
		}
		else if (point.z < m_Min.z)
		{
			m_Min.z = point.z;
		}
	}
	m_Max.z += m_Offset;
}

glm::vec3 CShadowBox::GetCenter() const
{
	float x = (m_Min.x + m_Max.x) / 2.f;
	float y = (m_Min.y + m_Max.y) / 2.f;
	float z = (m_Min.z + m_Max.z) / 2.f;
	glm::vec4 cen(x, y, z, 1);
	glm::mat4 inverted_light = glm::inverse(m_LightViewMatrix);
	return glm::vec3(inverted_light * cen);
}

float CShadowBox::GetWidth() const
{
	return m_Max.x - m_Min.x;
}

float CShadowBox::GetHeight() const
{
	return m_Max.y - m_Min.y;
}

float CShadowBox::GetLength() const
{
	return m_Max.z - m_Min.z;
}

std::vector<glm::vec4> CShadowBox::CalculateFrustumVertices(glm::mat4 rotation, glm::vec3 forward_vector, glm::vec3 center_near, glm::vec3 center_far) const
{
	glm::vec3 up_vector(rotation * m_Up);
	glm::vec3 right_vector = glm::cross(forward_vector, up_vector);
	glm::vec3 down_vector(-up_vector.x, -up_vector.y, -up_vector.z);
	glm::vec3 left_vector(-right_vector.x, -right_vector.y, -right_vector.z);
	glm::vec3 far_top = center_far + up_vector * m_FarHeight;
	glm::vec3 far_bottom = center_far + down_vector * m_FarHeight;
	glm::vec3 near_top = center_near + up_vector * m_NearHeight;
	glm::vec3 near_bottom = center_near + down_vector * m_NearHeight;
	std::vector<glm::vec4> points;
	points.push_back(CalculateLightSpaceFrustumCorner(far_top, right_vector, m_FarWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(far_top, left_vector, m_FarWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(far_bottom, right_vector, m_FarWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(far_bottom, left_vector, m_FarWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(near_top, right_vector, m_NearWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(near_top, left_vector, m_NearWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(near_bottom, right_vector, m_NearWidth));
	points.push_back(CalculateLightSpaceFrustumCorner(near_bottom, left_vector, m_NearWidth));
	return points;
}

glm::vec4 CShadowBox::CalculateLightSpaceFrustumCorner(const glm::vec3 & startPoint, const glm::vec3 & direction, const float & width) const
{
	glm::vec3 point = startPoint + glm::vec3(direction.x * width, direction.y * width, direction.z * width);
	glm::vec4 point4f(point.x, point.y, point.z, 1.0f);
	point4f = m_LightViewMatrix * point4f;
	return point4f;
}
