#pragma once
#include "../Shaders/EntityGeometryPassShader.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "../Shaders/LightPassShader.h"
#include "./EnitityRenderer.h"
#include "./SkyBoxRenderer.h"
#include "../Shadows/ShadowMapRenderer.h"
#include "./TerrainRenderer.h"
#include "../Engine/Scene.h"
#include "../Engine/OpenGLRenderer.h"
#include "../Water/WaterRenderer.h"
#include "DebugRenderer.h"
#include "../Shaders/GrassShader.h"
#include "../Shaders/FXAAShader.h"
#include "FrameBuffers/DeferedFrameBuffer.h"
#include "FrameBuffers/FilterFrameBuffer.h"

class CMasterRenderer
{
public:
	~CMasterRenderer();
	void ShadowPass(CScene* scene, const bool& shadows);
	void GeometryPass(CScene* scene, const bool& shadows);
	void LightPass(CScene* scene, glm::vec2 window_size, GLuint target);
	void FXAApass();
	void BindFinalPass();

	void UpdateBonesTransform(CScene* scene, CShaderProgram* shaderprogram);

	void RenderWaterTextures(CScene* scene, const bool& shadows);
	
	void Render(CScene* scene, const bool& shadows);

	void Init(CCamera* camera, glm::vec2 window_size, glm::mat4& projection_matrix,
		const float& fov, const float& near, const float& far, float rendering_resolution_modifier = 1.f, float shadow_map_size = 2048.f, float shadows_distance = 35,
		float water_quality = 1, glm::vec2 reflection_size = glm::vec2(320, 240), glm::vec2 refraction_size = glm::vec2(640, 480),
		float view_distance = 250
	);
	void SetSkyBoxTextures(CTextInfo* day, CTextInfo*  night);
	void SetSkyBoxMesh(const CMesh* mesh);
	const GLuint& GetShadowMap() const;
	CSkyBoxRenderer& GetSkyBoxRenderer();

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	GLuint m_TestDepthMap;

	CWaterRenderer	   m_WaterRenderer;
	CTerrainRenderer   m_TerrainRenderer;
	CEntityRenderer    m_EntityRenderer;
	CSkyBoxRenderer    m_SkyBoxRenderer;
	CShadowMapRenderer m_ShadowMapRenderer;
	CDebugRenderer	   m_DebugRenderer;

	CEntityGeometryPassShader	m_EntityGeometryPassShader;
	CTerrainGeometryPassShader	m_TerrainGeometryPassShader;
	CLightPassShader			m_LightPassShader;
	CGrassShader				m_GrassShader;
	CFXAAShader					m_FxaaShader;

	glm::mat4	m_ProjectionMatrix;
	glm::vec4	m_ClipPlane;
	glm::vec2	m_ReflectionSize;
	glm::vec2	m_RefractionSize;
	float		m_WaterQuality;

	float		m_ViewDistance;

	float		m_ShadowMapSize;
	float		m_ShadowsDistance;

	glm::vec2	m_WindowSize;
	
	CDefferedFrameBuffer m_DefferedFrameBuffer;
	CFilterFrameBuffer m_FilterFrameBuffer;
	//GLuint		m_Fbo;
	//GLuint		m_FinalTexture;
	//GLuint		m_DepthTexture;
	//GLuint		m_Textures[BufferTexture::Type::COUNT];

	int	   m_QuadIndicesSize;
	GLuint m_QuadVao;
	GLuint m_QuadIndices;
	GLuint m_QuadVertex;
	GLuint m_QuadTexCoord;	

	float m_ResoultionMultipler = .5f;

	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
};