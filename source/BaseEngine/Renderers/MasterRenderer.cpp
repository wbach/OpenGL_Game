#include "MasterRenderer.h"

void CMasterRenderer::Init(CCamera* camera, glm::vec2 window_size, glm::mat4& projection_matrix, 
	const float& fov, const float& near, const float& far, float rendering_resolution_modifier, float shadow_map_size, float shadows_distance,
	float water_quality, glm::vec2 reflection_size, glm::vec2 refraction_size,
	float view_distance
	)
{
	m_ProjectionMatrix = projection_matrix;

	m_WindowSize = window_size;

	m_ViewDistance = view_distance + 60;

	m_ShadowMapSize = shadow_map_size;
	m_ShadowsDistance = shadows_distance;

	m_ResoultionMultipler = rendering_resolution_modifier;

	m_DefferedFrameBuffer.Init(m_ResoultionMultipler*window_size);
	m_FilterFrameBuffer.Init(m_ResoultionMultipler*window_size);

	m_EntityGeometryPassShader.Init();
	m_EntityGeometryPassShader.Start();
	m_EntityGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_EntityGeometryPassShader.LoadViewDistance(view_distance + 50);
	m_EntityGeometryPassShader.Stop();

	m_TerrainGeometryPassShader.Init();
	m_TerrainGeometryPassShader.Start();
	m_TerrainGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_TerrainGeometryPassShader.LoadViewDistance(view_distance +50);
	m_TerrainGeometryPassShader.LoadUseNormalMap(1.0f);
	m_TerrainGeometryPassShader.Stop();

	m_LightPassShader.Init();
	m_LightPassShader.Start();
	m_LightPassShader.LoadSkyColour(glm::vec3(0.8));
	m_LightPassShader.LoadScreenSize(window_size);
	m_LightPassShader.LoadViewDistance(view_distance);
	m_LightPassShader.LoadProjectionMatrix(projection_matrix);
	m_LightPassShader.Stop();

	Utils::CreateQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord, m_QuadIndicesSize);

	m_SkyBoxRenderer.Init(projection_matrix, far);
	m_ShadowMapRenderer.Init(camera, window_size, fov, near, shadow_map_size, shadows_distance);

	m_ReflectionSize = reflection_size*  m_ResoultionMultipler;
	m_RefractionSize = refraction_size*  m_ResoultionMultipler;
	m_WaterQuality	 = water_quality;

	m_WaterRenderer.Init(window_size, projection_matrix, m_ReflectionSize , m_RefractionSize, m_WaterQuality);

	m_GrassShader.Init();
	m_GrassShader.Start();
	m_GrassShader.LoadProjectionMatrix(projection_matrix);
	m_GrassShader.LoadTransformMatrix(Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(1)));
	m_GrassShader.Stop();

	m_FxaaShader.Init();
	m_FxaaShader.Start();
	m_FxaaShader.LoadFxaaSpanMax(8.f);
	m_FxaaShader.LoadFxaaReduceMin(1.f / 128.f);
	m_FxaaShader.LoadFxaaReduceMul(0.f/*1.f / 8.f*/);
	m_FxaaShader.LoadScreenSize(window_size);
	m_FxaaShader.Stop();
}

CMasterRenderer::~CMasterRenderer()
{
	Utils::DeleteQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord);
}

void CMasterRenderer::ShadowPass(CScene* scene, const bool& shadows)
{
	if (!shadows)
		return;
	glDepthMask(GL_TRUE);
	m_ShadowMapRenderer.Render(scene);
}
void CMasterRenderer::RenderWaterTextures(CScene* scene, const bool& shadows)
{
	glEnable(GL_CLIP_DISTANCE0);
	m_WaterRenderer.SetIsRender(false);

	for (const auto& tile : scene->GetWaterTilesInCameraRange())
	{		
		glm::vec3 camera_position = scene->GetCamera()->GetPosition();
		float distance = 2 * (camera_position.y - tile->GetPosition().y);
		camera_position.y -= distance;
		scene->GetCamera()->SetPosition(camera_position);
		scene->GetCamera()->InvertPitch();
		scene->GetCamera()->UpdateViewMatrix();
		m_ClipPlane = glm::vec4(0, 1, 0, -tile->GetPosition().y + 0.0f);
		
		m_WaterRenderer.ChangeToReflectionViewPort();
		GeometryPass(scene, shadows);
		LightPass(scene, m_WindowSize, m_WaterRenderer.GetReflectionFrameBuffer());
		m_WaterRenderer.UnbindCurrentFrameBuffer();

		camera_position.y += distance;
		scene->GetCamera()->SetPosition(camera_position);
		scene->GetCamera()->InvertPitch();
		scene->GetCamera()->UpdateViewMatrix();

		m_WaterRenderer.ChangeToRefractionViewPort();
		m_ClipPlane = glm::vec4(0, 1, 0, tile->GetPosition().y - 0.0f);
		GeometryPass(scene, shadows);
		LightPass(scene, m_WindowSize, m_WaterRenderer.GetRefractionFrameBuffer());		

		m_WaterRenderer.UnbindCurrentFrameBuffer();
	}
	m_WaterRenderer.CopyTextureDepthTexture(m_DefferedFrameBuffer.GetFbo(), m_DefferedFrameBuffer.GetDepthTexture(), m_RefractionSize);

	m_WaterRenderer.SetIsRender(true);
	glDisable(GL_CLIP_DISTANCE0);
}
void CMasterRenderer::Render(CScene* scene, const bool& shadows)
{

	scene->CheckObjectsInCameraRange();
	
	
	if (m_WaterQuality > 0.5f)
	RenderWaterTextures(scene, shadows);

	m_ClipPlane = glm::vec4(0, 1, 0, 100000);

	ShadowPass(scene, shadows);
	//glViewport(0, 0, static_cast<int>(m_ResoultionMultipler*m_WindowSize.x), static_cast<int>(m_ResoultionMultipler *m_WindowSize.y));
	GeometryPass(scene, shadows);
	//glViewport(0, 0, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));	
	LightPass(scene, m_WindowSize, m_FilterFrameBuffer.GetFbo());
	BindFinalPass();
	FXAApass();



	//for (const shared_ptr<CEntity>& en : scene->GetEntities())
	//{
	//	m_DebugRenderer.DrawLineMesh(m_ProjectionMatrix * scene->GetCamera()->GetViewMatrix(), en->m_RigidBody.m_Colider.GetFaces());
	//}
	
}
void CMasterRenderer::GeometryPass(CScene* scene, const bool& shadows)
{
	m_DefferedFrameBuffer.BindToDraw();
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	std::list<CTerrain*> current_terrains = scene->GetTerrainsInCameraRange();

	// *********************************************SkyBox render******************************************
	m_SkyBoxRenderer.Render(scene->GetViewMatrix(), scene->m_DayNightCycle.GetDeltaTime(), scene->m_DayNightCycle.GetDayNightBlendFactor());
	//*****************************************************************************************************
	// ******************************Terrain render********************************************************
	if (!current_terrains.empty())
	{
		m_TerrainGeometryPassShader.Start();
		if (shadows)
		{
			glActiveTexture(GL_TEXTURE11);
			glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
			m_TerrainGeometryPassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
		}
		m_TerrainGeometryPassShader.LoadShadowValues(static_cast<float>(shadows), m_ShadowsDistance, m_ShadowMapSize);
		m_TerrainGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());
		m_TerrainRenderer.Render(scene, m_TerrainGeometryPassShader);
		m_TerrainGeometryPassShader.Stop();
	}	
	//****************************************************************************************************
	// **************************************Entities render**********************************************
	if (!scene->GetEntitiesInCameraRange().empty())
	{
		m_EntityGeometryPassShader.Start();
		if (shadows)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
			m_EntityGeometryPassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
		}
		m_EntityGeometryPassShader.LoadClipPlane(m_ClipPlane);
		m_EntityGeometryPassShader.LoadShadowValues(static_cast<float>(shadows), m_ShadowsDistance, m_ShadowMapSize);
		m_EntityGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());
		m_EntityRenderer.Render(scene, m_EntityGeometryPassShader);
		// **************************************Trees use entity render******************************************
		for (const auto& terrain : current_terrains)
		{
			for (auto& model : terrain->m_Trees)
			{
				CModel* m = scene->GetLoader().m_Models[model].get();
				m_EntityRenderer.RenderModel(scene, *m, m_EntityGeometryPassShader, m->GetInstancedSize());
			}
		}
		m_EntityGeometryPassShader.Stop();
	}
	
	//****************************************************************************************************
	
	//****************************************************************************************************

	// **************************************Grass(Flora) render******************************************	
	for (const auto& terrain : current_terrains)
	{
		if (terrain->m_Grass.empty())
			continue;
		m_GrassShader.Start();
		if (shadows)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
			m_GrassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
		}
		m_GrassShader.LoadGlobalTime(scene->m_GloabalTime);
		m_GrassShader.LoadShadowValues(static_cast<float>(shadows), m_ShadowsDistance, m_ShadowMapSize);
		m_GrassShader.LoadViewMatrix(scene->GetViewMatrix());
		for (CGrass& grass : terrain->m_Grass)
		{
			m_GrassShader.LoadViewDistance(grass.m_ViewDistance);
			grass.Render(scene->GetLoader().GetTextureLoader());
		}
		m_GrassShader.Stop();
	}
			
	//****************************************************************************************************
	// **************************************Water render******************************************
	if(!scene->GetWaterTilesInCameraRange().empty())
	m_WaterRenderer.Render(scene, scene->m_DayNightCycle.GetDeltaTime());

	m_RendererObjectPerFrame = m_EntityRenderer.GetObjectsPerFrame() + m_TerrainRenderer.GetObjectsPerFrame();
	m_RendererVertixesPerFrame = m_EntityRenderer.GetVertexPerFrame() + m_TerrainRenderer.GetVertexPerFrame();

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);	
}

void CMasterRenderer::LightPass(CScene* scene, glm::vec2 window_size, GLuint target)
{		
	//glEnable(GL_BLEND);
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_ONE, GL_ONE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);

	m_DefferedFrameBuffer.BindTextures();

	glClear(GL_COLOR_BUFFER_BIT);

	m_LightPassShader.Start();	
	m_LightPassShader.LoadViewMatrix(scene->GetViewMatrix());
	m_LightPassShader.LoadSkyColour(glm::vec3(0.8) * scene->m_DayNightCycle.GetDayNightBlendFactor());
	m_LightPassShader.LoadCameraPosition(scene->GetCameraPosition());
	m_LightPassShader.LoadLight(scene->GetDirectionalLight(), 0);
	int lights = scene->GetLights().size() + 1;
	m_LightPassShader.LoadLightNumber(lights);
	int i = 1;
	for (const auto& light : scene->GetLights())
	{
		m_LightPassShader.LoadLight(light, i++);
	}
	Utils::SimpleRenderVao(m_QuadVao, m_QuadIndicesSize);
	m_LightPassShader.Stop();

}
void CMasterRenderer::FXAApass()
{
	m_FxaaShader.Start();
	m_FilterFrameBuffer.BindTextures();
	Utils::SimpleRenderVao(m_QuadVao, m_QuadIndicesSize);
	m_FxaaShader.Stop();
}
void CMasterRenderer::BindFinalPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}
void CMasterRenderer::UpdateBonesTransform(CScene* scene, CShaderProgram * shaderprogram)
{

}
void CMasterRenderer::SetSkyBoxTextures(CTextInfo* day, CTextInfo*  night)
{
	m_SkyBoxRenderer.SetTextures(day, night);
}
void CMasterRenderer::SetSkyBoxMesh(const CMesh* mesh)
{
	m_SkyBoxRenderer.SetMesh(mesh);
}
const GLuint & CMasterRenderer::GetShadowMap() const
{
	return m_ShadowMapRenderer.GetShadowMap();
}
CSkyBoxRenderer& CMasterRenderer::GetSkyBoxRenderer()
{
	return m_SkyBoxRenderer;
}

const unsigned int& CMasterRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int& CMasterRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}