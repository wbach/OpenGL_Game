#pragma once
#include "Hero.h"
#include "../BaseEngine/Input/InputManager.h"

class CPlayer 
{
public:
	CPlayer(CInputManager* input_manager) : m_InputManager(input_manager) {}

	void SetPosition(const glm::vec3& position);

	void SetHero(std::shared_ptr<CHero> hero);
	std::shared_ptr<CHero>& GetHero();

	void Move(const float& delta_time);
	void CheckInputs();
private:

	CInputManager* m_InputManager;
	std::shared_ptr<CHero> m_Hero = nullptr;
};