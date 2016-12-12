#include "ShadowFrameBuffer.h"

CShadowFrameBuffer::~CShadowFrameBuffer()
{
	if (!m_IsInit) return;
	std::cout << "Destructor " << typeid(*this).name() << std::endl;
	glDeleteTextures(1, &m_ShadowMap);
	glDeleteFramebuffers(1, &m_Fbo);
}

void CShadowFrameBuffer::Init(glm::vec2 size, glm::vec2 window_size)
{
	m_Size = size;
	m_WindowSize = window_size;
	InitialiseFrameBuffer();
	m_IsInit = true;
}

void CShadowFrameBuffer::BindFBO()
{
	Utils::BindFrameBuffer(m_Fbo, static_cast<int>(m_Size.x), static_cast<int>(m_Size.y));
}

void CShadowFrameBuffer::UnbindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));
}

void CShadowFrameBuffer::InitialiseFrameBuffer()
{
	m_Fbo = Utils::CreateFrameBuffer();
	m_ShadowMap = Utils::CreateDepthBufferAttachment(static_cast<int>(m_Size.x), static_cast<int>(m_Size.y));
	UnbindFrameBuffer();
}

const GLuint & CShadowFrameBuffer::GetShadowMap() const
{
	return m_ShadowMap;
}
