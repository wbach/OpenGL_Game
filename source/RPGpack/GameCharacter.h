#pragma once
#include "../BaseEngine/Entities/Entity.h"
#include "CharacterStats.h"
#include "../BaseEngine/Engine/Scene.h"
namespace CharacterActions
{
	enum Type{
		IDLE = 0,
		RUN,
		WALK,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		SPELL_1,
		SPELL_2,
		SPELL_3,
		SPELL_4,
		COUNT
	};
}

class CGameCharacter : virtual public CEntity
{
public:
	//virtual void CharacterOpenningSpeech() = 0;
	CharacterActions::Type m_Action = CharacterActions::IDLE;
	SCharacterStats m_CharacterStats;	
};