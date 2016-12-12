#pragma once
#include "GameCharacter.h"
typedef long int lint;
class CHero : virtual public CGameCharacter
{
public:
	virtual void Create(CScene* scene);
	virtual void Move(const float& delta_time);
	void Jump();	
	CharacterActions::Type GetAction() { return m_Action; }
	void SetAction(CharacterActions::Type action);

	const lint& GetMeleExp() { return m_MeleExp; }
	const lint& GetDistanceExp() { return m_DistanceExp; }
	const lint& GetAbilityExp() { return m_AbilityExp; }
	const lint& GetStaminaExp() { return m_StaminaExp; }

	const virtual int GetAttackDamage();
	const virtual int GetMagickDamage();

	const virtual int GetArmor();
	const virtual int GetMegicResist();
protected:
	void SetModelId(CharacterActions::Type);

	int m_Lvl = 1;
	lint m_GeneralExp = 0;
	lint m_MeleExp = 0;
	lint m_DistanceExp = 0;
	lint m_AbilityExp = 0;
	lint m_StaminaExp = 0;

	unsigned int m_ModelsIds[CharacterActions::COUNT];
	std::string m_Models[CharacterActions::COUNT];
	CharacterActions::Type m_LastAction = CharacterActions::IDLE;
};