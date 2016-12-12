#include "GUITexture.h"


CGUITexture::CGUITexture(CTextInfo* texture, glm::vec2 position, glm::vec2 size)
: m_Texture(texture)
, CGUIElement(position, size)
, m_Shader(nullptr)
, m_QuadVao(nullptr)
{
	m_Type = GUIElements::TEXTURE;
}

void CGUITexture::SetShaders(GUIShader * shader, GLuint * quad)
{
	m_Shader = shader;
	m_QuadVao = quad;
}

CGUITexture::~CGUITexture()
{
}

void CGUITexture::SetRotation(const float rotation)
{
	m_Rotation = rotation;
}

const GLuint& CGUITexture::GetTextureId() const 
{ 
	return m_Texture->GetId();
}

void CGUITexture::SetTexture(CTextInfo* t)
{
	m_Texture = t; 
}

void CGUITexture::Save(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node) const
{
	rapidxml::xml_node<>* texture_node = document.allocate_node(rapidxml::node_element, "GUITexture");

	rapidxml::xml_node<>* name = document.allocate_node(rapidxml::node_element, "Name", m_Name.c_str());
	texture_node->append_node(name);

	//rapidxml::xml_node<>* file = document.allocate_node(rapidxml::node_element, "File", m_Texture->fullpath.c_str());
	//texture_node->append_node(file);

	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	glm::vec2 parent_pos = m_Parent != nullptr ? m_Parent->GetPosition() : glm::vec2(0);

	Utils::AddVectorToNode(document, position, m_Position + parent_pos);
	texture_node->append_node(position);

	rapidxml::xml_node<>* size = document.allocate_node(rapidxml::node_element, "Size");
	Utils::AddVectorToNode(document, size, m_Size);
	texture_node->append_node(size);

	node->append_node(texture_node);
}

CGUIElement * CGUITexture::Clone()
{
	CGUITexture* n = new CGUITexture(m_Texture, m_Position, m_Size);
	*n = *this;
	//n->SetShaders(m_Shader, m_QuadVao);
	//n->SetRotation(m_Rotation);
	return dynamic_cast<CGUIElement*>(n);
}

void CGUITexture::Update()
{
}

void CGUITexture::Render() const
{
	if (m_Shader == nullptr || m_QuadVao == nullptr)
		return;
	m_Shader->Start();
	glBindVertexArray(*m_QuadVao);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	if(m_Texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, m_Texture->GetId());
	m_Shader->LoadTransformMatrix(Utils::CreateTransformationMatrix(m_Position, m_Size, m_Rotation));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_Shader->Stop();
}

