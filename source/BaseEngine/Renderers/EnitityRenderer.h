#pragma once
#include "../Engine/Transform.h"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Entities/Entity.h"
#include "../Engine/Scene.h"
#include "../Shaders/EntityGeometryPassShader.h"
using namespace std;
class CEntityRenderer 
{	
public :
	CEntityRenderer() {}
	void RenderEntity(CScene* scene, CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader);
	void Render(CScene* scene, const CEntityGeometryPassShader& geomentry_shader);
	void RenderModel(CScene* scene, CModel& model, const CEntityGeometryPassShader& geomentry_shader,
		const int& transform_matrixes_size = 1,
		const std::vector<glm::mat4>& transform_matrixes = std::vector<glm::mat4>(),
		const glm::mat4& relative_matrix = glm::mat4(1.0f), const int& current_frame = 0
	);

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	void LoadUniforms(CModel & model, const CEntityGeometryPassShader & geomentry_shader,
		const int& transform_matrixes_size,
		const std::vector<glm::mat4>& transform_matrixes,
		const glm::mat4& relative_matrix);

	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
	unsigned int m_MaxVerices = 0;
};
