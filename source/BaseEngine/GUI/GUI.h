#pragma once
#include "Textures/GUITexture.h"
#include "Text/GUIText.h"
#include "Buttons/GUIButton.h"
#include "Text/Font.h"
#include "Text/FontShader.h"
#include "Window/GUIWindow.h"
#include <vector>
#include <list>

class CGUI
{
public:
	//Diffrend games can have other actions
	virtual void GUIActions() {}
	virtual void AssignActions() {}

	void AddElement(std::shared_ptr<CGUIElement> element);
	CGUIElement* GetElement(const unsigned int & id);
	CGUIElement* GetElement(const std::string & name);
	const GuiElementsList& GetAllElements() const;
	void Init(GUIShader* gui, FontShader* font_shader, CFont* font, GLuint* quad);
	void SetElements();
	void SetElement(std::shared_ptr<CGUIElement>& element);
	void Clear();
protected:
	GuiElementsList m_Elements;
	GuiElementsList m_TmpElements;
	
	GUIShader* m_Shader = nullptr;
	glm::vec2 m_WindowSize;

	//freetype::font_data m_TextFont;
	CFont*		m_TextFont   = nullptr;
	FontShader*	m_Fontshader = nullptr;
	GLuint*		m_QuadVao	 = nullptr;
};