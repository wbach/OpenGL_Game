#pragma once
#include "../SkyBoxRenderer.h"
#include "../../Shaders/Mobile/SimpleEnityShader.h"
#include "../../Shaders/Mobile/SimpleTerrainShader.h"
#include "../../Engine/Scene.h"

class CSimpleMasterRenderer
{
public:
	void Init(CCamera* camera, glm::vec2 window_size, glm::mat4& projection_matrix,	float view_distance = 250);
	void Render(CScene* scene);

	void SetSkyBoxTextures(CTextInfo* day, CTextInfo*  night);
	void SetSkyBoxMesh(const CMesh* mesh);
	void RenderEntity(CScene* scene, CEntity* entity, CModel & model, const int& instaced_size = 1) const;
	//CSkyBoxRenderer& GetSkyBoxRenderer();

protected:
	CSkyBoxRenderer  m_SkyBoxRenderer;

	CSimpleEntityShader m_SimpleEntityShader;
	CSimpleTerrainShader m_SimpleTerrainShader;

	glm::mat4	m_ProjectionMatrix;
	float		m_ViewDistance;
	glm::vec2	m_WindowSize;
};