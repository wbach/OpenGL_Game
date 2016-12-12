#pragma once
#include "../Buttons/GUIButton.h"

#include <list>

class CGUIWindow : public virtual CGUIElement
{
public: 
	CGUIWindow();
	virtual void Render() const override;
	void AddSubElement(std::shared_ptr<CGUIElement> element);
	void ClearSubElements();
	virtual GuiElementsList& GetSubElements();
	virtual void Save(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node) const override;
	virtual void SetPosition(const glm::vec2& position) override;
	virtual void SetSize(const glm::vec2& size) override;
	virtual CGUIElement* Clone() override;

	void Set(GUIShader* shader, GLuint* quad, CFont* font, FontShader* fontshader);
	void SetSubElements();
private:
	GuiElementsList m_SubElements;
	GUIShader* m_GuiShader = nullptr;
	GLuint* m_Quad = nullptr;
	CFont* m_Font = nullptr;
	FontShader* m_Fontshader = nullptr;
};