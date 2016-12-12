#include "GUIButton.h"


void CGUIButton::SetTextShaders(FontShader * fshader, CFont * font)
{
	m_Font = font;
	m_FShader = fshader;
}

void CGUIButton::SetText(std::string text)
{
	
}

int CGUIButton::CheckStatus(const glm::vec2& window_size)
{
	m_State = GuiButtonState::NORMAL;
	glm::vec2 mouse_pos = m_InputManager->GetMousePosition();
	float x = ((mouse_pos.x) / (float)window_size.x) * 2 - 1;
	float y = (1 - ((mouse_pos.y) / (float)window_size.y)) * 2 - 1;

	if (x >= m_Position.x - m_Size.x  && x < m_Position.x + m_Size.x)
	{
		if (y >= m_Position.y - m_Size.y  && y < m_Position.y + m_Size.y)
		{
			if (m_InputManager->GetMouseKey(KeyCodes::LMOUSE))
			{
				m_State = GuiButtonState::ACTIVE;
				if (OnClick != nullptr)
					OnClick();
			}
			else
				m_State =  GuiButtonState::HOVER;
		}
	}
	return m_State;
}

CGUIElement * CGUIButton::Clone()
{
	//CGUIButton* n = new CGUIButton();
	//*n = *this;
	return nullptr;
}

void CGUIButton::Render() const
{

}

void CGUIButton::Save(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node) const
{
}
