#pragma once
#include "../Input/InputManager.h"
#include "glm/glm/glm.hpp"
class CCharacterController
{
public:
	void Translate(glm::vec3 vec);
private:
	CInputManager* m_InputManager;
};