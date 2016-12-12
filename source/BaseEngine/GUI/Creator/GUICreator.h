#pragma once
#include "../Window/GUIWindow.h"
#include "../../Input/InputManager.h"
class CGUICreator
{
public:
	CGUICreator(CInputManager& input_manager);
	void SetElementToEdit(CGUIElement* element);
	void UpdateGuiElementPosition();
	void SetWindowSize(const glm::vec2& window_size);
private:
	CInputManager& m_InputManager;
	CGUIElement* m_CurrentElement;

	glm::vec2 m_StartPosition;
	glm::vec2 m_StartSize;

	glm::vec2 m_WindowSize;
};