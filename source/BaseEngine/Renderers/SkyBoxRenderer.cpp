#include "SkyBoxRenderer.h"

void CSkyBoxRenderer::Init(const glm::mat4& projection_matrix, const float& distance_view)
{
	m_DistanceView = distance_view;
	m_SkyBoxShader.Init();
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadProjectionMatrix(projection_matrix);
	m_SkyBoxShader.Stop();
}

void CSkyBoxRenderer::Init(const glm::mat4 & projection_matrix, const float & distance_view, char* vertex_shader, char* fragment_shader)
{
	m_DistanceView = distance_view;
	m_SkyBoxShader.Init(vertex_shader, fragment_shader);
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadProjectionMatrix(projection_matrix);
	m_SkyBoxShader.Stop();
}

void CSkyBoxRenderer::Render(const glm::mat4& view_matrix, const float&  delta_time, const float& blend_factor)
{
	if (m_QubeMesh == nullptr)
		return;

	if (!m_QubeMesh->IsReadyToRender())
		return;

	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadViewMatrix(view_matrix, delta_time, m_DistanceView);
	float color = 0.8f * blend_factor;
	m_SkyBoxShader.LoadFogColour(color, color, color);
	glBindVertexArray(m_QubeMesh->GetVao());
	glEnableVertexAttribArray(0);
	BindTextures(blend_factor);
	glDrawElements(GL_TRIANGLES, m_QubeMesh->GetVertexCount(), GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_SkyBoxShader.Stop();
}
void CSkyBoxRenderer::SetTextures(CTextInfo* day, CTextInfo* night)
{
	m_DayTexture	= day;
	m_NightTexture	= night;
}
void CSkyBoxRenderer::SetMesh(const CMesh* qube_mesh)
{
	m_QubeMesh = qube_mesh;
}
CTextInfo * CSkyBoxRenderer::GetDayTexture()
{
	return m_DayTexture;
}
CTextInfo * CSkyBoxRenderer::GetNightTexture()
{
	return m_NightTexture;
}
const CMesh * CSkyBoxRenderer::GetMesh()
{
	return m_QubeMesh;
}
void CSkyBoxRenderer::BindTextures(const float& blend_factor) const
{
	if (m_DayTexture == nullptr || m_NightTexture == nullptr)
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_DayTexture->GetId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_NightTexture->GetId());
	//cout << blend_factor << endl;
	m_SkyBoxShader.LoadBlendFactor(blend_factor);
}
