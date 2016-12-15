#pragma once
#include "ShaderProgram.h"

class CTerrainGeometryPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;
	void LoadShadowValues(const float& is, const float& distance, const float& shadow_map_size) const;
	void LoadViewDistance(const float& distance) const;
	void LoadUseNormalMap(const float& use) const;
private:
	void ConnectTextureUnits() const;
	
	//Samplers2d location
	int location_blendMap;
	int location_backgroundTexture;
	int location_rTexture;
	int location_bTexture;
	int location_gTexture;
	int location_RockTexture;

	int location_backgroundTextureNormal;
	int location_rTextureNormal;
	int location_bTextureNormal;
	int location_gTextureNormal;
	int location_RockTextureNormal;

	//shadows
	int location_ShadowVariables;
	int location_shadowMap;
	int location_ToShadowMapSpace;

	int location_UseNormalMap;

	// Matrixes location
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;

	//Skip render per distance
	int location_ViewDistance;
};
