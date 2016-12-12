#pragma once
#include "../Textures/GUITexture.h"
#include "../Text/GUIText.h"
#include "../../Input/InputManager.h"
#include <functional>
namespace GuiButtonState 
{
	enum {
		NORMAL = 0,
		HOVER,
		ACTIVE
	};
}

class CGUIButton : public virtual CGUIElement
{
public:	
	void SetTextShaders(FontShader* fshader, CFont* font);
	void SetText(std::string text);
	int CheckStatus(const glm::vec2& window_size);
	virtual CGUIElement* Clone() override;
	// Inherited via CGUIElement
	virtual void Render() const override;
	virtual void Save(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node) const override;
private:
	std::function<void()> OnClick = nullptr;
	int			m_State;
	glm::vec2&  m_WindowSize;

	std::shared_ptr<CGUITexture> m_NormalTexture = nullptr;
	std::shared_ptr<CGUITexture> m_HoverTexture	 = nullptr;
	std::shared_ptr<CGUITexture> m_ActiveTexture = nullptr;
	std::shared_ptr<CGUIText>	 m_Text			 = nullptr;
	CInputManager* m_InputManager;

	CFont* m_Font;
	FontShader* m_FShader;
};