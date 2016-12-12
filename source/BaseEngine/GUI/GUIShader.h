#pragma once
#include <vector>
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

class GUIShader : public CShaderProgram
{	
public:
	void Init();
	void GetAllUniformLocations() override;
	void BindAttributes() override;
	void LoadTransformMatrix(const glm::mat4& matrix) const;
	~GUIShader();
private:
	int location_transformationMatrix;
};