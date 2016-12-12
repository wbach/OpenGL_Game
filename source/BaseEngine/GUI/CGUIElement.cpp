#include "CGUIElement.h"
unsigned int CGUIElement::s_Id = 0;

CGUIElement::CGUIElement()
: CGUIElement(glm::vec2(0), glm::vec2(0))
{	
}
CGUIElement::CGUIElement(const glm::vec2 & position, const glm::vec2 & size)
: m_Position(position)
, m_Size(size)
, m_OrginalPosition(position)
, m_OrginalSize(size)
, m_Parent(nullptr)
{
	m_Id = s_Id++;
	m_Type = GUIElements::NONE;
}
void CGUIElement::SetParent(CGUIElement * p)
{
	m_Parent = p;
}
CGUIElement * CGUIElement::GetParent()
{
	return m_Parent;
}
const GUIElements::Type & CGUIElement::GetType()
{
	return m_Type;
}
const unsigned int & CGUIElement::GetId() const
{
	return m_Id;
}
const glm::vec2& CGUIElement::GetPosition() const
{
	return m_Position;
}

const glm::vec2& CGUIElement::GetSize() const
{
	return m_Size;
}

const glm::vec2 & CGUIElement::GetOrginalPosition() const
{
	return m_OrginalPosition;
}

const glm::vec2 & CGUIElement::GetOrginalSize() const
{
	return m_OrginalSize;
}

const std::string CGUIElement::GetName() const
{
	return m_Name;
}

void CGUIElement::SetName(const std::string & name)
{
	m_Name = name;
}

void CGUIElement::SetPosition(const glm::vec2 & position)
{
	m_Position = position;
}

void CGUIElement::SetSize(const glm::vec2 & size)
{
	m_Size = size;
}

void CGUIElement::IncrasePosition(const glm::vec2 & offset)
{
	m_Position += offset;
}

void CGUIElement::IncraseSize(const glm::vec2 & offset)
{
	m_Size += offset;
}
