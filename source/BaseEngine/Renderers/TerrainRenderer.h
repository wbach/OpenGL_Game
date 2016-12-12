#pragma once
#include <vector>
#include "../Terrain/Terrain.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "../Engine/Scene.h"
class CTerrainRenderer
{	
public:
    void Render(CScene* scene, const CTerrainGeometryPassShader& geomentry_shader);
    void PrepareTerrain(CScene* scene, const CTerrain &terrain);
    void BindTextures(CScene* scene, const CTerrain &terrain);
    void UnBindTextureModel();
    void LoadModelMatrix(const CTerrain &terrain, const CTerrainGeometryPassShader& geomentry_shader);
	void RenderElements(CTerrain &terrain);

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	glm::mat4 m_TransformationMatrix;
	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
};