#pragma once
#include <gl/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "../CGUIElement.h"
#include "../GUIShader.h"
#include "../../Loader/Texture.h"

class CGUITexture : virtual public CGUIElement
{	
public:
	CGUITexture(CTextInfo* texture, glm::vec2 position, glm::vec2 scale);
	void SetShaders(GUIShader* shader, GLuint* quad);
	~CGUITexture();
	void SetRotation(const float rotation);
	virtual void Update() override;
	virtual void Render() const override;
	virtual void Save(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node) const override;
	virtual CGUIElement* Clone() override;

	const GLuint& GetTextureId() const;
	
	void SetTexture(CTextInfo* t);
private:
	CTextInfo*	m_Texture = nullptr;
	GUIShader*	m_Shader = nullptr;
	GLuint*		m_QuadVao = nullptr;
	float		m_Rotation = 0;
};
