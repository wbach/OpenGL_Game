#include "TerrainRenderer.h"

void CTerrainRenderer::Render(CScene* scene, const CTerrainGeometryPassShader& geomentry_shader)
{
	m_RendererObjectPerFrame = 0;
	m_RendererVertixesPerFrame = 0;

	for (const auto& terrain : scene->GetTerrainsInCameraRange())
	{
		if (!terrain->m_Model.GetMeshes()[0].IsReadyToRender() || !terrain->m_IsInit)
			continue;

		PrepareTerrain(scene, *terrain);
		LoadModelMatrix(*terrain, geomentry_shader);
		glDrawElements(GL_TRIANGLE_STRIP, terrain->m_Model.GetMeshes()[0].GetVertexCount(), GL_UNSIGNED_SHORT, 0);
		UnBindTextureModel();
		m_RendererObjectPerFrame++;
		m_RendererVertixesPerFrame += terrain->m_Model.GetMeshes()[0].GetVertexCount();
	}
}
const unsigned int& CTerrainRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int& CTerrainRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}
void CTerrainRenderer::RenderElements(CTerrain &terrain)
{


}
void CTerrainRenderer::PrepareTerrain(CScene* scene, const CTerrain &terrain)
{	
	glBindVertexArray(terrain.m_Model.GetMeshes()[0].GetVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	BindTextures(scene, terrain) ;
	// shader.loadShineVariables(0,0);
}

void CTerrainRenderer::BindTextures(CScene* scene, const CTerrain &terrain)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[ terrain.m_BackgroundTexture[0]]->GetId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_RTexture[0]]->GetId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_GTexture[0]]->GetId());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_BTexture[0]]->GetId());
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_BlendMap]->GetId());

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_BackgroundTexture[1]]->GetId());
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_RTexture[1]]->GetId());
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_GTexture[1]]->GetId());
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_BTexture[1]]->GetId());

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_RockTexture[0]]->GetId());
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, scene->GetLoader().m_Textures[terrain.m_RockTexture[1]]->GetId());
}

void CTerrainRenderer::UnBindTextureModel()
{
	glDisable(GL_TEXTURE_2D);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void CTerrainRenderer::LoadModelMatrix(const CTerrain &terrain, const CTerrainGeometryPassShader& geomentry_shader)
{
	m_TransformationMatrix = Utils::CreateTransformationMatrix(glm::vec3(terrain.m_Transform.position.x, 0, terrain.m_Transform.position.z), glm::vec3(0), glm::vec3(1));
	geomentry_shader.LoadTransformMatrix(m_TransformationMatrix);
}
