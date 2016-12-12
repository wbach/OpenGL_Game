#include "ThridPersonCamera.h"

CThirdPersonCamera::CThirdPersonCamera(CInputManager* input_manager, glm::vec3& look_at_position, glm::vec3& look_at_rotation)
: m_Offset(0.0f)
, m_LookAtPosition(look_at_position)
, m_LookAtRotation(look_at_rotation)
, m_Mousevel(0.2f)
, m_CaptureMouse(true)
, m_IsShowCursor(false)
, m_InputManager(input_manager)
{
	m_DistanceFromPlayer = 4.0f;
}

void CThirdPersonCamera::SetCaptureMouse(bool capture)
{
	m_CaptureMouse = capture;
}
void CThirdPersonCamera::LockCamera()
{
	if(m_Pitch > 90.0f)
		m_Pitch = 90.0f;
	if(m_Pitch < -90.0f)
		m_Pitch = -90.0f;
	if(m_Yaw < 0.0f)
		m_Yaw += 360.0f;
	if(m_Yaw > 360.0f)
		m_Yaw -= 360.0f;
}

void CThirdPersonCamera::AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) {
	m_LookAtPosition = position_entity;
	m_LookAtRotation = rotation_entity;
}

void CThirdPersonCamera::CalculateInput()
{
	glm::vec2 d_move = CalcualteMouseMove() * m_Mousevel;
	CalculatePitch(d_move);
	CalculateAngleAroundPlayer(d_move);
	LockCamera();
}

void CThirdPersonCamera::Move()
{	
	float horizontal_distance = CalculateHorizontalDistance() ;
	float vertical_distance	 = CalculateVerticalDistance() ;
	CalculateCameraPosition(horizontal_distance, vertical_distance);
	this->m_Yaw = 180 - (m_LookAtRotation.y + m_AngleAroundPlayer) ;

	__super::Move();
}
void CThirdPersonCamera::SetPosition(glm::vec3 position)
{
	m_Position = position;	
}
void CThirdPersonCamera::CalculateCameraPosition(float horizontal_distance, float vertical_distance)
{
	float theata  = m_LookAtRotation.y  + m_AngleAroundPlayer;
	float x_offset = (float) (horizontal_distance * sin(Utils::ToRadians(theata))) ;
	float z_offset = (float) (horizontal_distance * cos(Utils::ToRadians(theata))) ;
	m_Position.x  = m_LookAtPosition.x - x_offset;
	m_Position.y  = m_LookAtPosition.y + vertical_distance + 1.8f;
	m_Position.z  = m_LookAtPosition.z - z_offset;
	m_Position	 += m_Offset;
}

float CThirdPersonCamera::CalculateHorizontalDistance()
{
	return (float) (m_DistanceFromPlayer * cos(Utils::ToRadians(m_Pitch))) ;
}

float CThirdPersonCamera::CalculateVerticalDistance()
{
	return (float) (m_DistanceFromPlayer * sin(Utils::ToRadians(m_Pitch))) ;
}

void CThirdPersonCamera::CalculateZoom(float zoom_lvl)
{
	this->m_DistanceFromPlayer += zoom_lvl;
}

glm::vec2 CThirdPersonCamera::CalcualteMouseMove()
{
	return m_InputManager->CalcualteMouseMove();
}

void CThirdPersonCamera::CalculatePitch(glm::vec2 d_move)
{
		m_Pitch -= d_move.y;
}

void CThirdPersonCamera::CalculateAngleAroundPlayer(glm::vec2 d_move)
{
	float angle_change	 = d_move.x;
	m_AngleAroundPlayer -= angle_change;
}
