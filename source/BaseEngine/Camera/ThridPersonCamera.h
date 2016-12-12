#pragma once
#include "../Input/InputManager.h"
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>

class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(CInputManager *input_manager, glm::vec3& look_at_positiom, glm::vec3& look_at_rotation);

	void CalculateInput() override;
	void Move() override;
	void AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) override;
	void CalculateZoom(float zoom_lvl) override;
private:
	void LockCamera();
	void SetCaptureMouse(bool capture);	
	void CalculateCameraPosition(float horizontal_distance, float vertical_distance);
	void SetPosition(glm::vec3 position);
	float CalculateHorizontalDistance();
	float CalculateVerticalDistance();

	glm::vec2 CalcualteMouseMove();
	void CalculatePitch(glm::vec2 d_move);
	void CalculateAngleAroundPlayer(glm::vec2 d_move);

	glm::vec3&	m_LookAtPosition;
	glm::vec3&	m_LookAtRotation;

	bool		m_IsShowCursor;
	glm::vec3	m_Offset;
	float		m_Mousevel;
	bool		m_CaptureMouse;
	int			m_WindowWidth, m_WindowHeight;
	CInputManager *m_InputManager;
};
