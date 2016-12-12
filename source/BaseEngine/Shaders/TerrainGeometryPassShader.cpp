#include "TerrainGeometryPassShader.h"



void CTerrainGeometryPassShader::Init()
{
	InitShaderProgram("Data/Shaders/Terrain/TerrainGeometryPassShader.vert", "Data/Shaders/Terrain/TerrainGeometryPassShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CTerrainGeometryPassShader::ConnectTextureUnits() const
{
	LoadValue(location_backgroundTexture, 0);
	LoadValue(location_rTexture, 1);
	LoadValue(location_gTexture, 2);
	LoadValue(location_bTexture, 3);
	LoadValue(location_blendMap, 4);
	LoadValue(location_backgroundTextureNormal, 5);
	LoadValue(location_rTextureNormal, 6);
	LoadValue(location_gTextureNormal, 7);
	LoadValue(location_bTextureNormal, 8);
	LoadValue(location_RockTexture, 9);
	LoadValue(location_RockTextureNormal, 10);
	LoadValue(location_shadowMap, 11);	
}
void CTerrainGeometryPassShader::GetAllUniformLocations()
{
	// Get location of matrixes
	location_transformationMatrix = GetUniformLocation("TransformationMatrix");
	location_projectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_viewMatrix = GetUniformLocation("ViewMatrix");

	//Get location of samplers 2d
	location_blendMap = GetUniformLocation("BlendMap");

	location_backgroundTexture = GetUniformLocation("BackgroundTexture");
	location_rTexture = GetUniformLocation("rTexture");
	location_bTexture = GetUniformLocation("bTexture");
	location_gTexture = GetUniformLocation("gTexture");
	
	location_backgroundTextureNormal = GetUniformLocation("BackgroundTextureNormal");
	location_rTextureNormal = GetUniformLocation("rTextureNormal");
	location_bTextureNormal = GetUniformLocation("bTextureNormal");
	location_gTextureNormal = GetUniformLocation("gTextureNormal");

	location_RockTexture = GetUniformLocation("RockTexture");
	location_RockTextureNormal = GetUniformLocation("RockTextureNormal");

	//Distance
	location_ViewDistance = GetUniformLocation("ViewDistance");

	//Shadows
	location_ShadowVariables = GetUniformLocation("ShadowVariables");
	location_shadowMap		  = GetUniformLocation("ShadowMap");
	location_ToShadowMapSpace = GetUniformLocation("ToShadowMapSpace");
}

void CTerrainGeometryPassShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(2, "Normal");
	BindAttribute(3, "Tangent");
}
void CTerrainGeometryPassShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_transformationMatrix, matrix);
}

void CTerrainGeometryPassShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_projectionMatrix, matrix);
}

void CTerrainGeometryPassShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_viewMatrix, matrix);
}

void CTerrainGeometryPassShader::LoadToShadowSpaceMatrix(const glm::mat4 & matrix) const
{
	LoadValue(location_ToShadowMapSpace, matrix);
}
void CTerrainGeometryPassShader::LoadShadowValues(const float& is, const float& distance, const float& shadow_map_size) const
{
	LoadValue(location_ShadowVariables, glm::vec3(is, distance - 5, shadow_map_size));
}
void CTerrainGeometryPassShader::LoadViewDistance(const float& distance) const
{
	LoadValue(location_ViewDistance, distance);
}

