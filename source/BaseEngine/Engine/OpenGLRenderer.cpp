#include "OpenGLRenderer.h"


COpenGLRenderer::COpenGLRenderer()
{
}

void COpenGLRenderer::InitializeRenderer()
{
	GLint glew_init_result = glewInit();
	if (glew_init_result != GLEW_OK)
	{
		printf("ERROR: %s\n", glewGetErrorString(glew_init_result));
		exit(-1);
	}
	printf("GL version: %s\n\n", glGetString(GL_VERSION));
}

void COpenGLRenderer::CalculateFPS()
{
}

void COpenGLRenderer::Clear(int c)
{
	//glClear()
}

