#pragma once
#include <vector>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Camera/Camera.h"
#include "../Lights/Light.h"
class LoadingShader : public CShaderProgram 
{
public:
	LoadingShader() {}
	void Init();
	void LoadIsTextured(const float&) const;	
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
	void LoadAlphaValue(const float&) const;
	void ConnectTextureUnits() const;

	void GetAllUniformLocations() override;
	void BindAttributes() override;

	~LoadingShader();
private:
	int location_isTextured;
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_modelTexture;
	int location_alpha;
};