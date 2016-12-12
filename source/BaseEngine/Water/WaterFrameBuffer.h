#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"

class CWaterFrameBuffers
{
public:	
	~CWaterFrameBuffers();
	void Init(glm::vec2 window_size, glm::vec2 reflection_size, glm::vec2 refraction_size);
	void InitialiseReflectionFrameBuffer();

	void InitialiseRefractionFrameBuffer();
	void ChangeToReflectionViewPort();
	void ChangeToRefractionViewPort();
	void UnbindCurrentFrameBuffer() const;

	void CopyTextureDepthTexture(const GLuint& source_fbo, const GLuint& source_text, const glm::vec2& source_size);

	const GLuint& GetReflectionFrameBuffer() const;
	const GLuint& GetRefractionFrameBuffer() const;

	const GLuint& GetReflectionTexture() const;
	const GLuint& GetRefractionTexture() const;
	const GLuint& GetRefractionDepthTexture() const;
private:
	void ChangeViewPort(float width, float height);
	GLuint CreateFrameBuffer() const;
	GLuint CreateTextureAttachment(const glm::vec2& size) const;
	GLuint CreateDepthTextureAttachment(const glm::vec2& size) const;
	GLuint CreateDepthBufferAttachment(const glm::vec2& size) const;	

	glm::vec2 m_WindowSize;

	glm::vec2 m_ReflectionSize;
	glm::vec2 m_RefractionSize;

	GLuint m_ReflectionFrameBuffer;
	GLuint m_ReflectionTexture;
	GLuint m_ReflectionDepthBuffer;

	GLuint m_RefractionFrameBuffer;
	GLuint m_RefractionTexture;
	GLuint m_RefractionDepthTexture;

	bool m_IsInitialized = false;
};