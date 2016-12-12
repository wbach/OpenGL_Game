#pragma once
#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>
#include <thread>
#include "glm/glm.hpp"
#include <memory>

class COpenGLRenderer : public CRenderer
{
public:
	COpenGLRenderer();
	virtual void InitializeRenderer() override;	
	virtual void CalculateFPS() override;
private:


	// Inherited via CRenderer
	virtual void Clear(int c) override;

};