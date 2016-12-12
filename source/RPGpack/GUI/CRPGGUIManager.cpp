#include "CRPGGUIManager.h"

void CRPGGUIManager::ApplyInputs()
{
	if (m_InputManager.GetKeyDown(GameActions::GUI_STATS_WINDOW))
	{
		if(m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW] != nullptr)
			m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW]->m_IsVisible = !m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW]->m_IsVisible;
	}
	if (m_InputManager.GetKeyDown(GameActions::GUI_INVENTORY_WINDOW))
	{
		if (m_GUIElements[RPGGuiElements::GUI_INVENTORY_WINDOW] != nullptr)
			m_GUIElements[RPGGuiElements::GUI_INVENTORY_WINDOW]->m_IsVisible = !m_GUIElements[RPGGuiElements::GUI_INVENTORY_WINDOW]->m_IsVisible;
	}
}

void CRPGGUIManager::RenderHerosBars(const glm::mat4 & projection_matrix, const glm::mat4 & view_matrix, const std::list<CGameObject*> objects)
{
	CGUIElement* el = m_GUIElements[RPGGuiElements::HP_HERO_BAR];
	m_TmpElements.clear();
	m_GUIElements[RPGGuiElements::HP_HERO_BAR]->m_IsVisible = false;
	for (auto& e : objects)
	{
		glm::mat4 pv = projection_matrix * view_matrix;
		glm::vec4 pos = pv * glm::vec4(e->GetWorldPosition() + glm::vec3(0, 2, 0), 1.f);
		cout << pos.z << " " << pos.w << std::endl;
		//pos.z = -pos.z;
		float distance = pos.z;
		if (pos.z < 0 || distance > 30)
			continue;
		pos /= pos.w;
		
		
			glm::vec2 p(pos.x, pos.y);
		
		//Utils::PrintVector("V: ", el->GetOrginalSize() *  pos.z);

		el->m_IsVisible = true;
		el->SetPosition(p);
		//el->SetSize(el->GetOrginalSize() *  pos.z);
		m_TmpElements.push_back(std::shared_ptr<CGUIElement>(el->Clone()));
		el->m_IsVisible = false;
	}
}

void CRPGGUIManager::Update(CHero * player)
{
	UpdateStatsWindow(player);
	UpdateCompas(player);
	UpdateBars(player);
}

void CRPGGUIManager::UpdateStatsWindow(CHero * player)
{
	if (m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW] == nullptr)
		return;

	CGUIWindow* window = dynamic_cast<CGUIWindow*>(m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW]);
	if (window == nullptr)
		return;

	for (auto& element : window->GetSubElements())
	{
		CGUIText* text = dynamic_cast<CGUIText*>(element.get());
		if (text == nullptr)
			continue;
		
		//  Strength
		if (!text->GetName().compare("Strength"))
		{
			text->UpdateText("Strength: " + std::to_string(player->m_CharacterStats.m_Strength));
			continue;
		}
		if (!text->GetName().compare("MeleExp"))
		{
			text->UpdateText("Mele Exp: " + std::to_string(player->GetMeleExp()));
			continue;
		}

		//Agility
		if (!text->GetName().compare("Agility"))
		{
			text->UpdateText("Agility: " + std::to_string(player->m_CharacterStats.m_Agility));
			continue;
		}
		if (!text->GetName().compare("RangeExp"))
		{
			text->UpdateText("Range exp: " + std::to_string(player->GetDistanceExp()));
			continue;
		}

		//Stamina
		if (!text->GetName().compare("Vitality"))
		{
			text->UpdateText("Vitality: " + std::to_string(player->m_CharacterStats.m_Vitality));
			continue;
		}
		if (!text->GetName().compare("StaminaExp"))
		{
			text->UpdateText("Stamina exp: " + std::to_string(player->GetStaminaExp()));
			continue;
		}

		//Ability
		if (!text->GetName().compare("Energy"))
		{
			text->UpdateText("Energy: " + std::to_string(player->m_CharacterStats.m_Energy));
			continue;
		}
		if (!text->GetName().compare("AbilityExp"))
		{
			text->UpdateText("Ability exp: " + std::to_string(player->GetAbilityExp()));
			continue;
		}

		//Damge
		if (!text->GetName().compare("AttckDamge"))
		{
			text->UpdateText("Attck damge: " + std::to_string(player->GetAttackDamage()));
			continue;
		}
		if (!text->GetName().compare("MagicDamge"))
		{
			text->UpdateText("Magic damge: " + std::to_string(player->GetMagickDamage()));
			continue;
		}

		//Resist
			if (!text->GetName().compare("Armor"))
			{
				text->UpdateText("Armor: " + std::to_string(player->GetArmor()));
				continue;
			}
		if (!text->GetName().compare("MagicResist"))
		{
			text->UpdateText("Magic resist: " + std::to_string(player->GetMegicResist()));
			continue;
		}
	}
}

void CRPGGUIManager::UpdateCompas(CHero * player)
{
	if (m_GUIElements[RPGGuiElements::GUI_COMPAS] == nullptr || player == nullptr)
		return;

	CGUITexture* comp = dynamic_cast<CGUITexture*> (m_GUIElements[RPGGuiElements::GUI_COMPAS]);
	
	if (comp != nullptr)
	{
		comp->SetRotation(-player->GetRotation().y);
	}
}

void CRPGGUIManager::UpdateBars(CHero * hero)
{
	if (m_GUIElements[RPGGuiElements::HP_BAR] == nullptr || hero == nullptr)
		return;

	float percent = static_cast<float>(hero->m_CharacterStats.m_CurrentHealth) / static_cast<float>(hero->m_CharacterStats.m_MaxHealth);

	glm::vec2 o_position = m_GUIElements[RPGGuiElements::HP_BAR]->GetOrginalPosition();
	glm::vec2 old_size = m_GUIElements[RPGGuiElements::HP_BAR]->GetSize();
	glm::vec2 o_size = m_GUIElements[RPGGuiElements::HP_BAR]->GetOrginalSize();
	float diff = (1.f - percent) * o_size.x;
	m_GUIElements[RPGGuiElements::HP_BAR]->SetSize(glm::vec2(o_size.x  * percent, old_size.y));
	m_GUIElements[RPGGuiElements::HP_BAR]->SetPosition(glm::vec2(o_position.x  - diff, o_position.y));
}

void CRPGGUIManager::GUIActions()
{
	ApplyInputs();
}

void CRPGGUIManager::AssignActions()
{
	for (auto& el : m_Elements)
	{
		if (el->GetName().compare("Stats") == 0)
		{
			m_GUIElements[RPGGuiElements::CHARACTER_STATISTIC_WINDOW] = el.get();
			continue;
		}		

		if (el->GetName().compare("HeroBar") == 0)
		{
			m_GUIElements[RPGGuiElements::HP_HERO_BAR] = el.get();
			el->m_IsVisible = false;
			continue;
		}

		CGUIWindow* window = dynamic_cast<CGUIWindow*>(el.get());
		if (window == nullptr)
			continue;

		for (auto& sel : window->GetSubElements())
		{
			if (sel->GetName().compare("Compas") == 0)
			{
				m_GUIElements[RPGGuiElements::GUI_COMPAS] = sel.get();
				continue;
			}
			if (sel->GetName().compare("HPbar") == 0)
			{
				m_GUIElements[RPGGuiElements::HP_BAR] = sel.get();
				continue;
			}
		}		
	}
}

CRPGGUIManager::CRPGGUIManager(CInputManager & input_manager)
	: m_InputManager(input_manager)
{
	for (auto& e : m_GUIElements)
	{
		e = nullptr;
	}


}
