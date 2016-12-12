#pragma once
#include <GL/glew.h>
#include "../../Utils/Utils.h"
#include <vector>

class CFrameBuffer
{
public:
	virtual void Init(const glm::vec2& size) = 0;
	void CreateFrameBuffer();
	void AddTexture(GLuint& texture);
	void SetDepthTexture(const GLuint& texture);
	int CheckStatus();
	const GLuint& GetFbo();
	const GLuint& GetDepthTexture();
	const GLuint& GetTexture(const unsigned int& id);
	void BindTextures(int offset = 0);
	void BindToDraw();
	void Bind();
	void UnBind();
	void UnBindDraw();
	~CFrameBuffer();
protected:
	bool m_IsInitialized = false;
	GLuint m_Fbo;
	GLuint m_DepthTexture;
	std::vector<GLuint> m_Textures;
	bool m_DepthStorage = false;
};