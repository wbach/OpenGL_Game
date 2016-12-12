#pragma once
#include "../Camera/Camera.h"
#include "glm/glm.hpp"
#include <vector>
#include <memory>
class CShadowBox 
{	
public:
	CShadowBox();
	void Init(CCamera* camera, glm::vec2 window_size, float fov, float near_plane, float shadows_distance);
	void Update();
	glm::vec3 GetCenter() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetLength() const;
	const float GetAspectRatio() const;	
	void SetLightViewMatrix(const glm::mat4& matrix);
private:
	void CalculateWidthsAndHeights();
	glm::mat4 CalculateCameraRotationMatrix() const;
	std::vector<glm::vec4>  CalculateFrustumVertices(glm::mat4 rotation, glm::vec3 forward_vector, glm::vec3 center_near, glm::vec3 center_far) const;
	glm::vec4 CalculateLightSpaceFrustumCorner(const glm::vec3& startPoint, const glm::vec3& direction, const float& width) const;

	glm::vec2	m_WindowSize;
	glm::vec3	m_Min;
	glm::vec3	m_Max;

	glm::mat4	m_LightViewMatrix;
	CCamera*	m_Camera;

	float		m_FarHeight,
				m_FarWidth, 
				m_NearHeight,
				m_NearWidth;
	float		m_Fov,
				m_NearPlane;

	float		m_Offset;
	glm::vec4	m_Up;
	glm::vec4	m_Forward;
	float		m_ShadowDistance;
};