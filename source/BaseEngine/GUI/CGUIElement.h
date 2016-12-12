#pragma once
#include "../Utils/Utils.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include <string>
#include <memory>
#include <list>
class CGUIElement;
typedef std::list<std::shared_ptr<CGUIElement>> GuiElementsList;

namespace GUIElements
{
	enum Type
	{
		NONE = 0,
		TEXTURE,
		TEXT,
		BUTTON,
		WINDOW
	};
}

class CGUIElement
{
public:
	CGUIElement();
	CGUIElement(const glm::vec2& position, const glm::vec2& size);
	virtual void Update() {};
	virtual void Render() const = 0;
	virtual void Save(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node) const = 0;

	void SetParent(CGUIElement* p);
	CGUIElement* GetParent();
	virtual CGUIElement* Clone() = 0;

	const GUIElements::Type& GetType();
	const unsigned int& GetId() const;
	const glm::vec2& GetPosition() const;
	const glm::vec2& GetSize() const;
	const glm::vec2& GetOrginalPosition() const;
	const glm::vec2& GetOrginalSize() const;
	const std::string GetName() const;

	virtual void SetName(const std::string& name);
	virtual void SetPosition(const glm::vec2& position);
	virtual void SetSize(const glm::vec2& size);
	virtual void IncrasePosition(const glm::vec2& offset);
	virtual void IncraseSize(const glm::vec2& offset);

	bool m_IsVisible = true;
	bool m_SaveToFile = true;
protected:
	glm::vec2 m_OrginalPosition;
	glm::vec2 m_Position;
	glm::vec2 m_Size;
	glm::vec2 m_OrginalSize;
	std::string m_Name;

	GUIElements::Type m_Type;
	unsigned int m_Id;
	static unsigned int s_Id;

	CGUIElement* m_Parent;
};