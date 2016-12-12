#include "GUIWindow.h"

CGUIWindow::CGUIWindow()
{
	m_Type = GUIElements::WINDOW;
}

void CGUIWindow::Render() const
{
	for (const auto& t : m_SubElements)
	{
		if (t->m_IsVisible)
			t->Render();
	}
}

void CGUIWindow::AddSubElement(std::shared_ptr<CGUIElement> element)
{
	m_SubElements.push_back(element);
}

void CGUIWindow::ClearSubElements()
{
	m_SubElements.clear();
}

GuiElementsList& CGUIWindow::GetSubElements()
{
	return m_SubElements;
}

void CGUIWindow::Save(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node) const
{
	rapidxml::xml_node<>* window_node = document.allocate_node(rapidxml::node_element, "GUIWindow");

	rapidxml::xml_node<>* name = document.allocate_node(rapidxml::node_element, "Name", m_Name.c_str());
	window_node->append_node(name);

	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	Utils::AddVectorToNode(document, position, m_Position);
	window_node->append_node(position);

	rapidxml::xml_node<>* size = document.allocate_node(rapidxml::node_element, "Size");
	Utils::AddVectorToNode(document, size, m_Size);
	window_node->append_node(size);

	for (const auto& e : m_SubElements)
	{
		e->Save(document, window_node);
	}	
	node->append_node(window_node);
}

void CGUIWindow::SetPosition(const glm::vec2 & position)
{
	glm::vec2 dmove = position - m_Position;
	for (auto& t : m_SubElements)
	{
		t->IncrasePosition(dmove);
	}
	CGUIElement::SetPosition(position);
}

void CGUIWindow::SetSize(const glm::vec2 & size)
{
	glm::vec2 dsize = size - m_Size;
	for (auto& t : m_SubElements)
	{
		t->IncraseSize(dsize);
	}
	CGUIElement::SetSize(size);
}

CGUIElement * CGUIWindow::Clone()
{
	CGUIWindow* n = new CGUIWindow();
	*n = *this;
	n->ClearSubElements();
	for (auto& e : m_SubElements)
	{
		n->AddSubElement(shared_ptr<CGUIElement>(e->Clone()));
	}
	return dynamic_cast<CGUIElement*>(n);
}

void CGUIWindow::Set(GUIShader * shader, GLuint * quad, CFont * font, FontShader * fontshader)
{
	m_GuiShader = shader;
	m_Quad = quad;
	m_Font = font;
	m_Fontshader = fontshader;
}

void CGUIWindow::SetSubElements()
{
	for (auto& e : m_SubElements)
	{
		CGUITexture* texture = dynamic_cast<CGUITexture*>(e.get());
		if (texture != nullptr)
			texture->SetShaders(m_GuiShader, m_Quad);

		CGUIText* text = dynamic_cast<CGUIText*>(e.get());
		if (text != nullptr)
			text->SetShaders(m_Fontshader, m_Font);
	}
}
