#pragma once
#include <math.h>
#include <GL/glew.h>
#include "../Loader/Model.h"
#include "../Shaders/SkyBoxShader.h"

class CSkyBoxRenderer 
{
public:
	void Init(const glm::mat4& projection_matrix, const float& distance_view);
	void Init(const glm::mat4& projection_matrix, const float& distance_view, char* vertex_shader, char* fragment_shader);
	void Render(const glm::mat4& view_matrix, const float& delta_time, const float& blend_factor);
	void SetTextures(CTextInfo* day, CTextInfo* night);
	void SetMesh(const CMesh* m_QubeMesh);

	CTextInfo* GetDayTexture();
	CTextInfo* GetNightTexture();
	const CMesh*	   GetMesh();
private:
	void BindTextures(const float& blend_factor) const;
	float m_DistanceView = 100;
	CSkyBoxShader m_SkyBoxShader;

	CTextInfo* m_DayTexture;
	CTextInfo* m_NightTexture;

	const CMesh* m_QubeMesh = nullptr;
};