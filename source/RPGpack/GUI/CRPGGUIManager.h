#pragma once
#include "../../BaseEngine/GUI/GUI.h"
#include "../../BaseEngine/Input/InputManager.h"
#include "../Player.h"
namespace RPGGuiElements
{
	enum Type
	{
		CHARACTER_STATISTIC_WINDOW = 0,
		GUI_INVENTORY_WINDOW,
		GUI_COMPAS,
		HP_BAR,
		HP_HERO_BAR,
		WORLD_MAP,
		WORLD_MAP_POINTER,
		COUNT
	};
}

class CRPGGUIManager : public CGUI
{
public:
	CRPGGUIManager(CInputManager& input_manager);
	void ApplyInputs();	
	void RenderHerosBars(const glm::mat4& projection_matrix, const glm::mat4& view_matrix, const std::list<CGameObject*> objects);
	void Update(CHero* player);
	void UpdateStatsWindow(CHero* player);
	void UpdateCompas(CHero* player);
	void UpdateBars(CHero* hero);
	void UpdateMapPointer(CHero* hero);
	virtual void GUIActions() override;
	virtual void AssignActions() override;
		
	CGUIElement* m_GUIElements[RPGGuiElements::COUNT];
	CInputManager& m_InputManager;

	float m_TerrainSize = 1000;
};