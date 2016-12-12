#include "GUICreator.h"
#include <iostream>
CGUICreator::CGUICreator(CInputManager & input_manager)
	: m_InputManager(input_manager)
	, m_CurrentElement(nullptr)
{
}

void CGUICreator::SetElementToEdit(CGUIElement * element)
{
	if (element == nullptr) return;
	m_CurrentElement = element;
	m_StartPosition = element->GetPosition();
	m_StartSize = element->GetSize();
}

void CGUICreator::UpdateGuiElementPosition()
{
	if (m_CurrentElement == nullptr)
		return;

	if (m_InputManager.GetKey(KeyCodes::LCTRL))
	{
		glm::vec2 mouse_position = m_InputManager.GetMousePosition();
		glm::vec2 new_pos = Utils::toScreenSpace(mouse_position, m_WindowSize);
		m_CurrentElement->SetPosition(new_pos);
	}
	if (m_InputManager.GetKey(KeyCodes::SPACE))
	{
		m_CurrentElement->SetPosition(m_StartPosition);
	}
}
void CGUICreator::SetWindowSize(const glm::vec2 & window_size)
{
	m_WindowSize = window_size;
}