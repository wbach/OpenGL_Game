#pragma once
#include "../ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

class CSimpleTerrainShader : public CShaderProgram
{
public:
	void Init();
	void GetAllUniformLocations();
	void BindAttributes() override;
	void LoadTransformMatrix(const glm::mat4& matrix) const;
	void LoadProjectionMatrix(const glm::mat4& matrix) const;
	void LoadViewMatrix(const glm::mat4& matrix) const;
	void ConnectTextureUnits() const;

private:
	int location_TransformationMatrix;
	int location_ProjectionMatrix;
	int location_ViewMatrix;

	//Samplers2d location
	int location_blendMap;
	int location_backgroundTexture;
	int location_rTexture;
	int location_bTexture;
	int location_gTexture;
	int location_RockTexture;
};