#include "GUI.h"

const GuiElementsList& CGUI::GetAllElements() const
{
	if(m_TmpElements.empty())
		return m_Elements;

	GuiElementsList tmp_list;// = m_Elements;
	for (auto& e : m_Elements)
		tmp_list.push_back(e);
	for (auto& e : m_TmpElements)
		tmp_list.push_back(e);

	return tmp_list;
}

void CGUI::Init(GUIShader * gui, FontShader * font_shader, CFont * font, GLuint* quad)
{
	m_Shader	 = gui;
	m_Fontshader = font_shader;
	m_TextFont	 = font;
	m_QuadVao	 = quad;
}

void CGUI::AddElement(std::shared_ptr<CGUIElement> element)
{
	SetElement(element);
	m_Elements.push_back(element);
}

CGUIElement* CGUI::GetElement(const unsigned int & id)
{
	for (auto& e : m_Elements)
	{
		if (e->GetId() == id)
		{
			return e.get();
		}
		CGUIWindow* w = dynamic_cast<CGUIWindow*>(e.get());
		if (w != nullptr)
		{
			for (auto& se : w->GetSubElements())
			{
				if (se->GetId() == id)
				{
					return se.get();
				}
			}
		}
	}
	return nullptr;
}

CGUIElement * CGUI::GetElement(const std::string & name)
{
	for (auto& e : m_Elements)
	{
		if (e->GetName().compare(name) == 0)
		{
			return e.get();
		}
		CGUIWindow* w = dynamic_cast<CGUIWindow*>(e.get());
		if (w != nullptr)
		{
			for (auto& se : w->GetSubElements())
			{
				if (se->GetName().compare(name) == 0)
				{
					return se.get();
				}
			}
		}		
	}
	return nullptr;
}


void CGUI::SetElements()
{
	for (auto& e : m_Elements)
		SetElement(e);
}

void CGUI::SetElement(std::shared_ptr<CGUIElement>& element)
{
	CGUIText* text = dynamic_cast<CGUIText*>(element.get());
	if (text != nullptr)
	{
		text->SetShaders(m_Fontshader, m_TextFont);
		return;
	}
	CGUITexture* texture = dynamic_cast<CGUITexture*>(element.get());
	if (texture != nullptr)
	{
		texture->SetShaders(m_Shader, m_QuadVao);
		return;
	}

	CGUIButton* button = dynamic_cast<CGUIButton*>(element.get());
	if (button != nullptr)
	{
		//text->SetShaders(m_Fontshader, m_TextFont);
		return;
	}

	CGUIWindow* window = dynamic_cast<CGUIWindow*>(element.get());
	if (window != nullptr)
	{
		window->Set(m_Shader, m_QuadVao, m_TextFont, m_Fontshader);
		for(auto& se : window->GetSubElements())
			SetElement(se);
		return;
	}
}

void CGUI::Clear()
{
	m_Elements.clear();
}
