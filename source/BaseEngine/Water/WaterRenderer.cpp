#include "WaterRenderer.h"

void CWaterRenderer::ChangeToReflectionViewPort()
{
	m_WaterFrameBuffer.ChangeToReflectionViewPort();
}

void CWaterRenderer::ChangeToRefractionViewPort()
{
	m_WaterFrameBuffer.ChangeToRefractionViewPort();
}

const GLuint & CWaterRenderer::GetReflectionFrameBuffer() const
{
	return m_WaterFrameBuffer.GetReflectionFrameBuffer();
}

const GLuint & CWaterRenderer::GetRefractionFrameBuffer() const
{
	return m_WaterFrameBuffer.GetRefractionFrameBuffer();
}

const GLuint & CWaterRenderer::GetDepthTexture() const
{
	return m_WaterFrameBuffer.GetRefractionDepthTexture();
}

void CWaterRenderer::CopyTextureDepthTexture(const GLuint & source_fbo, const GLuint & source_text, const glm::vec2 & source_size)
{
	m_WaterFrameBuffer.CopyTextureDepthTexture(source_fbo, source_text, source_size);
}

void CWaterRenderer::UnbindCurrentFrameBuffer()
{
	m_WaterFrameBuffer.UnbindCurrentFrameBuffer();
}

CWaterRenderer::~CWaterRenderer()
{
}

void CWaterRenderer::Init(glm::vec2 WindowSize, glm::mat4 projectionMatrix , glm::vec2 reflection_map_size, glm::vec2 refraction_map_size, float quality)
{
	m_Quality = quality;
	m_IsInitialized = true;
	m_Shader.Init();
	m_WaterFrameBuffer.Init(WindowSize, reflection_map_size, refraction_map_size);

	m_Shader.Start();
	m_Shader.ConnectTextureUnits();
	m_Shader.LoadProjectionMatrix(projectionMatrix);
	m_Shader.Stop();
}



void CWaterRenderer::Render(CScene* scene, const float& delta_time)
{
	if (!m_IsInitialized || !m_Render)
		return;

	m_Shader.Start();
	m_Shader.LoadViewMatrix(scene->GetCamera()->GetViewMatrix(), scene->GetCamera()->GetPosition());
	//m_Shader.LoadLight(scene->GetDirectionalLightPosition(), scene->GetLights()[0].GetColour());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (const auto& tile : scene->GetWaterTilesInCameraRange())
	{
		glBindVertexArray(tile->GetMeshVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_WaterFrameBuffer.GetReflectionTexture());
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_WaterFrameBuffer.GetRefractionTexture());
		
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tile->GetDudvTexture());
		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tile->GetNormalTexture());

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_WaterFrameBuffer.GetRefractionDepthTexture());

		m_Shader.LoadMoveFactor(tile->GetMoveFactor());
		tile->Update(delta_time);
		m_Shader.LoadTransformMatrix(tile->GetTransformMatrix());
		m_Shader.LoadWaterColor(tile->GetColor());
		m_Shader.LoadQuailty(m_Quality);

		glDrawElements(GL_TRIANGLES, tile->GetIndicesSize(), GL_UNSIGNED_SHORT, 0);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	glDisable(GL_BLEND);
	m_Shader.Stop();
}
