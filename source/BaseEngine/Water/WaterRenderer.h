#pragma once
#include "WaterShader.h"
#include "WaterFrameBuffer.h"
#include "WaterTile.h"
#include "../Engine/Scene.h"

class CWaterRenderer 
{	
public:	
	~CWaterRenderer();
	void Init(glm::vec2 WindowSize, glm::mat4 projectionMatrix, glm::vec2 reflection_map_size, glm::vec2 refraction_map_size, float quality);
	void Render(CScene* scene,const float& delta_time);

	void ChangeToReflectionViewPort();
	void ChangeToRefractionViewPort();

	const GLuint& GetReflectionFrameBuffer() const;
	const GLuint& GetRefractionFrameBuffer() const;
	const GLuint& GetDepthTexture() const;
	void CopyTextureDepthTexture(const GLuint& source_fbo, const GLuint& source_text, const glm::vec2& source_size);

	void UnbindCurrentFrameBuffer();
	void SetIsRender(bool is) 
	{
		m_Render = is;
	}
private:
	float m_Quality;
	CWaterFrameBuffers	m_WaterFrameBuffer;
	CWaterShader		m_Shader;

	bool   m_Render = true;
	bool   m_IsInitialized = false;
};