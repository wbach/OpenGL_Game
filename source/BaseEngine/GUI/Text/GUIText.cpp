#include "GUIText.h"

CGUIText::CGUIText(std::string text, glm::vec2 position, float font_size, glm::vec3 colour)
: CGUIElement(position, glm::vec2(font_size))
, m_Font(nullptr)
, m_Text(text)
, m_Colour(colour)
, m_Shader(nullptr)
{
	m_Type = GUIElements::TEXT;
}

void CGUIText::SetShaders(FontShader * shader, CFont * font)
{
	m_Shader = shader;
	m_Font = font;
}

void CGUIText::UpdateText(std::string text)
{
	m_Text = text;
}

CGUIElement * CGUIText::Clone()
{
	CGUIText*n = new CGUIText(m_Text, m_Position, m_Size.x, m_Colour);
	//n->SetShaders(m_Shader, m_Font);
	*n = *this;
	return dynamic_cast<CGUIElement*>(n);
}

void CGUIText::Update()
{
}

void CGUIText::Render() const
{
	if (m_Shader == nullptr || m_Font == nullptr)
		return;
	m_Shader->Start();
	glActiveTexture(GL_TEXTURE0);
	glPushMatrix();
	glLoadIdentity();
	GLfloat active_color[] = { m_Colour.x, m_Colour.y, m_Colour.z, 1 };
	glColor4fv(active_color);
	m_Shader->loadTranslation(m_Position);
	glScalef(m_Size.x, m_Size.x, m_Size.x);
	m_Font->Print(static_cast<const int>(m_Position.x), static_cast<const int>(m_Position.y), m_Text.c_str());
	glPopMatrix();
	m_Shader->Stop();
}

void CGUIText::Save(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node) const
{
	rapidxml::xml_node<>* text_node = document.allocate_node(rapidxml::node_element, "GUIText");

	rapidxml::xml_node<>* name = document.allocate_node(rapidxml::node_element, "Name", m_Name.c_str());
	text_node->append_node(name);

	rapidxml::xml_node<>* text = document.allocate_node(rapidxml::node_element, "Text", m_Text.c_str());
	text_node->append_node(text);

	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	Utils::AddVectorToNode(document, position, m_Position);
	text_node->append_node(position);

	rapidxml::xml_node<>* color = document.allocate_node(rapidxml::node_element, "Color");
	Utils::AddVectorToNode(document, color, m_Colour);
	text_node->append_node(color);

	rapidxml::xml_node<>* fsize = document.allocate_node(rapidxml::node_element, "FontSize", document.allocate_string(to_string(m_Size.x).c_str()));
	text_node->append_node(fsize);

	node->append_node(text_node);
}
