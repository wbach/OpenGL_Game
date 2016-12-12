#include "Knight.h"
#include <iostream>
#include "../../BaseEngine/Engine/GLgame.h"

void CKnight::Create(CScene* scene)
{
	//Data/Meshes/Garen/garen_idle.fbx
	m_Models[CharacterActions::IDLE]="Data/Meshes/Garen/garen_idle.fbx";
	m_Models[CharacterActions::RUN]="Data/Meshes/Garen/garen_run.fbx";
	m_Models[CharacterActions::WALK] = "Data/Meshes/Garen/garen_spell1.fbx";
	m_Models[CharacterActions::ATTACK_1] = "Data/Meshes/Garen/garen_attack.fbx";
	m_Models[CharacterActions::ATTACK_2] = "Data/Meshes/Garen/garen_attack2.fbx";
	m_Models[CharacterActions::ATTACK_3] = "Data/Meshes/Garen/garen_attack3.fbx";
	m_Models[CharacterActions::SPELL_1] = "Data/Meshes/Garen/garen_spell1.fbx";
	m_Models[CharacterActions::SPELL_2] = "Data/Meshes/Garen/garen_spell2.fbx";
	m_Models[CharacterActions::SPELL_3] = "Data/Meshes/Garen/garen_spell3.fbx";
	m_Models[CharacterActions::SPELL_4] = "Data/Meshes/Garen/garen_spell4.fbx";

	SetName("Player-Knight");
	
	CHero::Create(scene);
	std::cout << "Knight created." << std::endl;
}
