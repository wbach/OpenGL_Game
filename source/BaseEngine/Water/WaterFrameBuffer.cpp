#include "WaterFrameBuffer.h"
#include <iostream>
CWaterFrameBuffers::~CWaterFrameBuffers()
{
	if (!m_IsInitialized)
		return;
	std::cout << "Destructor " << typeid(*this).name() << std::endl;
	glDeleteTextures(1, &m_ReflectionTexture);
	glDeleteFramebuffers(1, &m_ReflectionFrameBuffer);
	glDeleteRenderbuffers(1, &m_ReflectionDepthBuffer);

	glDeleteTextures(1, &m_RefractionTexture);
	glDeleteTextures(1, &m_RefractionDepthTexture);
	glDeleteFramebuffers(1, &m_RefractionFrameBuffer);
}

void CWaterFrameBuffers::Init(glm::vec2 window_size, glm::vec2 reflection_size, glm::vec2 refraction_size)
{
	m_WindowSize = window_size;
	m_ReflectionSize = reflection_size;
	m_RefractionSize = refraction_size;

	InitialiseReflectionFrameBuffer();
	InitialiseRefractionFrameBuffer();

	m_IsInitialized = true;
}

void CWaterFrameBuffers::InitialiseReflectionFrameBuffer()
{
	m_ReflectionFrameBuffer = CreateFrameBuffer();
	m_ReflectionTexture		= CreateTextureAttachment(m_ReflectionSize);
	m_ReflectionDepthBuffer = CreateDepthBufferAttachment(m_ReflectionSize);
	UnbindCurrentFrameBuffer();
}

void CWaterFrameBuffers::InitialiseRefractionFrameBuffer()
{
	m_RefractionFrameBuffer  = CreateFrameBuffer();
	m_RefractionTexture		 = CreateTextureAttachment(m_RefractionSize);
	m_RefractionDepthTexture = CreateDepthTextureAttachment(m_RefractionSize);
	UnbindCurrentFrameBuffer();
}

void CWaterFrameBuffers::ChangeToReflectionViewPort()
{
	ChangeViewPort(m_ReflectionSize.x, m_ReflectionSize.y);
}

void CWaterFrameBuffers::ChangeToRefractionViewPort()
{
	ChangeViewPort(m_RefractionSize.x, m_RefractionSize.y);
}

const GLuint & CWaterFrameBuffers::GetReflectionTexture() const
{
	return m_ReflectionTexture;
}

const GLuint & CWaterFrameBuffers::GetRefractionTexture() const
{
	return m_RefractionTexture;
}

const GLuint & CWaterFrameBuffers::GetRefractionDepthTexture() const
{
	return m_RefractionDepthTexture;
}

void CWaterFrameBuffers::ChangeViewPort(float width, float height)
{
	glViewport(0, 0, static_cast<int>(width), static_cast<int>(height));
}

GLuint CWaterFrameBuffers::CreateFrameBuffer() const
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	//generate name for frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//create the framebuffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//indicate that we will always render to color attachment 0
	return frameBuffer;
}

GLuint CWaterFrameBuffers::CreateTextureAttachment(const glm::vec2 & size) const
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(size.x), static_cast<int>(size.y), 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,	texture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	return texture;
}

GLuint CWaterFrameBuffers::CreateDepthTextureAttachment(const glm::vec2 & size) const
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, static_cast<int>(size.x), static_cast<int>(size.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

GLuint CWaterFrameBuffers::CreateDepthBufferAttachment(const glm::vec2 & size) const
{
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(size.x), static_cast<int>(size.y));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	return depthBuffer;
}

void CWaterFrameBuffers::UnbindCurrentFrameBuffer() const
{
	//call to switch to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));
}

void CWaterFrameBuffers::CopyTextureDepthTexture(const GLuint& source_fbo, const GLuint& source_text, const glm::vec2& source_size)
{
	// bind fbo as read / draw fbo
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RefractionFrameBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source_fbo);

	// bind source texture to color attachment
	glBindTexture(GL_TEXTURE_2D, source_text);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, source_text, 0);
	//glDrawBuffer(GL_DEPTH_COMPONENT);

	// bind destination texture to another color attachment
	glBindTexture(GL_TEXTURE_2D, m_RefractionDepthTexture);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_RefractionDepthTexture, 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source_text, 0);
	//glReadBuffer(GL_DEPTH_COMPONENT);


	// specify source, destination drawing (sub)rectangles. 
	glBlitFramebuffer(0, 0, static_cast<int>(source_size.x), static_cast<int>(source_size.y),
		0, 0, static_cast<int>(m_RefractionSize.x), static_cast<int>(m_RefractionSize.y), GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// release state
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

const GLuint & CWaterFrameBuffers::GetReflectionFrameBuffer() const
{
	return m_ReflectionFrameBuffer;
}

const GLuint & CWaterFrameBuffers::GetRefractionFrameBuffer() const
{
	return m_RefractionFrameBuffer;
}
