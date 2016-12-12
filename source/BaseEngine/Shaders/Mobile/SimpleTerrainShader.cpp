#include "SimpleTerrainShader.h"

void CSimpleTerrainShader::Init()
{
	InitShaderProgram("Data/Shaders/Simple/SimpleTerrainShader.vert", "Data/Shaders/Simple/SimpleTerrainShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}

void CSimpleTerrainShader::GetAllUniformLocations()
{
	location_TransformationMatrix = GetUniformLocation("TransformationMatrix");
	location_ProjectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_ViewMatrix = GetUniformLocation("ViewMatrix");
	
	//Get location of samplers 2d
	location_blendMap = GetUniformLocation("BlendMap");

	location_backgroundTexture = GetUniformLocation("BackgroundTexture");
	location_rTexture = GetUniformLocation("rTexture");
	location_bTexture = GetUniformLocation("bTexture");
	location_gTexture = GetUniformLocation("gTexture");
	location_RockTexture = GetUniformLocation("RockTexture");
}

void CSimpleTerrainShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(4, "TransformationMatrixes");
}
void CSimpleTerrainShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_TransformationMatrix, matrix);
}

void CSimpleTerrainShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ProjectionMatrix, matrix);
}

void CSimpleTerrainShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ViewMatrix, matrix);
}

void CSimpleTerrainShader::ConnectTextureUnits() const
{
	LoadValue(location_backgroundTexture, 0);
	LoadValue(location_rTexture, 1);
	LoadValue(location_gTexture, 2);
	LoadValue(location_bTexture, 3);
	LoadValue(location_blendMap, 4);
	LoadValue(location_RockTexture, 5);
}
