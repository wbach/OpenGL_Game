#pragma once
#include "../Shaders/ShaderProgram.h"

class CWaterShader : public CShaderProgram 
{
public:
	CWaterShader();
	void Init();
	void BindAttributes() override;
	void GetAllUniformLocations() override;
	void LoadLight(const glm::vec3& position, const glm::vec3& colour) const;
	void LoadMoveFactor(const float& moveFactor) const;
	void LoadQuailty(const float& lvl) const;
	void ConnectTextureUnits()  const;
	void LoadProjectionMatrix(const glm::mat4& projection) const;
	void LoadViewMatrix(const glm::mat4& view_matrix, const glm::vec3 camera_position) const;
	void LoadTransformMatrix(const glm::mat4& modelMatrix) const;
	void LoadWaterColor(const glm::vec4& water_color) const;
	~CWaterShader() {}
private:
	int location_WaterColor;
	int location_TransformMatrix;
	int location_viewMatrix;
	int location_projectionMatrix;
	int location_reflectionTexture;
	int location_refractionTexture;
	int location_dudvMap;
	int location_moveFactor;
	int location_cameraPosition;
	int location_normalMap;
	int location_lightPosition;
	int location_lightColour;
	int location_depthMap;
	int location_lowQuality;
};