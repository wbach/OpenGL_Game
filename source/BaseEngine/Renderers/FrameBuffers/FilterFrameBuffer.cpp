#include "FilterFrameBuffer.h"

void CFilterFrameBuffer::Init(const glm::vec2 & size)
{
	CreateFrameBuffer();
	BindToDraw();
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<int>(size.x), static_cast<int>(size.y), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	AddTexture(texture);

	glGenRenderbuffers(1, &m_DepthTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(size.x), static_cast<int>(size.y));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthTexture);
	m_DepthStorage = true;
	CheckStatus();
	UnBindDraw();
}
