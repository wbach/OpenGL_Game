#pragma once
#include <GL/glew.h>
#include "../Utils/Utils.h"

class CShadowFrameBuffer
{	
public:
	~CShadowFrameBuffer();
	void Init(glm::vec2 size, glm::vec2 window_size);
	void BindFBO();
	void UnbindFrameBuffer() const;
	void InitialiseFrameBuffer();
	const GLuint& GetShadowMap() const;
private:
	GLuint m_Fbo;
	GLuint m_ShadowMap;

	glm::vec2 m_Size;
	glm::vec2 m_WindowSize;

	bool m_IsInit = false;
};