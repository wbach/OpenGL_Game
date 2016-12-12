#include "Player.h"

void CPlayer::SetPosition(const glm::vec3 & position)
{
	if (m_Hero == nullptr)
		return;

	m_Hero->SetPosition(position);
}

void CPlayer::SetHero(std::shared_ptr<CHero> hero)
{
	m_Hero = hero;
}

std::shared_ptr<CHero>& CPlayer::GetHero()
{
	return m_Hero;
}

void CPlayer::Move(const float & delta_time)
{
	if (m_Hero == nullptr)
		return;

	CheckInputs();
	m_Hero->Move(delta_time);
}

void CPlayer::CheckInputs()
{
	if (m_InputManager == nullptr) return;

	bool move_key_pres = false;

	if (m_InputManager->GetKey(GameActions::MOVE_FORWARD))
	{
		m_Hero->m_CharacterStats.m_CurrentMoveSpeed = m_Hero->m_CharacterStats.m_RunSpeed;
		m_Hero->SetAction(CharacterActions::RUN);
		move_key_pres = true;
	}
	else if (m_InputManager->GetKey(GameActions::MOVE_BACKWARD))
	{
		m_Hero->m_CharacterStats.m_CurrentMoveSpeed = -m_Hero->m_CharacterStats.m_RunSpeed;
		m_Hero->SetAction(CharacterActions::RUN);
		move_key_pres = true;
	}
	else if (m_InputManager->GetKey(GameActions::ATTACK_1))
	{
		m_Hero->SetAction(CharacterActions::ATTACK_1);
	}
	else if (m_InputManager->GetKey(GameActions::ATTACK_2))
	{
		m_Hero->SetAction(CharacterActions::ATTACK_2);
	}
	else if (m_InputManager->GetKey(GameActions::ATTACK_3))
	{
		m_Hero->SetAction(CharacterActions::ATTACK_3);
	}
	else
	{
		m_Hero->SetAction(CharacterActions::IDLE);
	}

	if (!move_key_pres)
		m_Hero->m_CharacterStats.m_CurrentMoveSpeed = 0;

	if (m_InputManager->GetKey(KeyCodes::D))
	{
		m_Hero->m_CharacterStats.m_CurrentTurnSpeed = -m_Hero->m_CharacterStats.m_TurnSpeed;
	}
	else if (m_InputManager->GetKey(KeyCodes::A))
	{
		m_Hero->m_CharacterStats.m_CurrentTurnSpeed = m_Hero->m_CharacterStats.m_TurnSpeed;
	}
	else
	{
		m_Hero->m_CharacterStats.m_CurrentTurnSpeed = 0;
	}

	if (m_InputManager->GetKey(GameActions::JUMP))
	{
		m_Hero->Jump();
	}
}
