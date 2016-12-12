#include "Grass.h"

CGrass::~CGrass()
{
	if (!m_IsInitialized)
		return;

	glDeleteBuffers(1, &m_VertexVbo);
	glDeleteVertexArrays(1, &m_MeshVao);

	m_IsInitialized = false;
}

void CGrass::Init(const std::vector<glm::vec3>& positions, const int& texture)
{
	
	m_GrassTexture = texture;
	grassvertex.clear();
	for(const glm::vec3& position : positions)
		{
			grassvertex.push_back(position.x);
			grassvertex.push_back(position.y);
			grassvertex.push_back(position.z);
		}
	
}

void CGrass::Render(CTextureLoader& loader) const
{
	if (!m_IsInitialized)
		return;

	Utils::DisableCulling();
	//scene->GetLoader().LoadTexture("Data/Textures/G3_Nature_Plant_Grass_06_Diffuse_01.png")
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, loader.GetTexture(m_GrassTexture)->GetId());
		
	glBindVertexArray(m_MeshVao);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, m_VertexSize);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	Utils::EnableCulling();
}

void CGrass::OpenGLLoadingPass()
{
	m_IsInitialized = true;
	m_MeshVao = Utils::CreateVao();
	m_VertexSize = grassvertex.size() / 3;
	m_VertexVbo = Utils::StoreDataInAttributesList(0, 3, grassvertex);
	Utils::UnbindVao();
	grassvertex.clear();
}
