#pragma once
#include "../CGUIElement.h"
#include "Font.h"
#include "FontShader.h"


class CGUIText : virtual public CGUIElement
{	
public:
	CGUIText(std::string text, glm::vec2 position, float font_size, glm::vec3 colour);
	void SetShaders(FontShader* shader, CFont* font);
	const std::string& GetText() const { return m_Text; }
	void UpdateText(std::string text);
	virtual CGUIElement* Clone() override;

	// Inherited via CGUIElement
	virtual void Update() override;
	virtual void Render() const override;
	virtual void Save(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node) const override;
private:
	std::string m_Text;
	glm::vec3 m_Colour;

	CFont* m_Font;
	FontShader* m_Shader;
};
