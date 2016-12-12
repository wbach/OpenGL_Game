#include "WaterShader.h"

CWaterShader::CWaterShader() 
{
}

void CWaterShader::Init() 
{
	InitShaderProgram("Data/Shaders/Water/waterVertex.vert", "Data/Shaders/Water/waterFragment.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}

void CWaterShader::BindAttributes()
{
	BindAttribute(0, "position");
}

void CWaterShader::GetAllUniformLocations()
{
	location_projectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_viewMatrix = GetUniformLocation("ViewMatrix");
	location_TransformMatrix = GetUniformLocation("TransformationMatrix");

	location_reflectionTexture = GetUniformLocation("reflectionTexture");
	location_refractionTexture = GetUniformLocation("refractionTexture");
	location_dudvMap = GetUniformLocation("dudvMap");
	location_moveFactor = GetUniformLocation("moveFactor");
	location_cameraPosition = GetUniformLocation("cameraPosition");
	location_normalMap = GetUniformLocation("normalMap");
	location_lightColour = GetUniformLocation("lightColour");
	location_lightPosition = GetUniformLocation("lightPosition");
	location_depthMap = GetUniformLocation("depthMap");
	location_lowQuality = GetUniformLocation("Quality");

	location_WaterColor = GetUniformLocation("WaterColor");
}

void CWaterShader::LoadLight(const glm::vec3 & position, const glm::vec3 & colour) const
{
	LoadValue(location_lightPosition, position);
	LoadValue(location_lightColour, colour);
}

void CWaterShader::LoadMoveFactor(const float & moveFactor) const
{
	LoadValue(location_moveFactor, moveFactor);
}

void CWaterShader::LoadQuailty(const float & lvl) const
{
	LoadValue(location_lowQuality, lvl);
}

void CWaterShader::ConnectTextureUnits() const
{
	LoadValue(location_reflectionTexture, 0);
	LoadValue(location_refractionTexture, 1);
	LoadValue(location_dudvMap, 2);
	LoadValue(location_normalMap, 3);
	LoadValue(location_depthMap, 4);
}

void CWaterShader::LoadProjectionMatrix(const glm::mat4 & projection) const
{
	LoadValue(location_projectionMatrix, projection);
}

void CWaterShader::LoadViewMatrix(const glm::mat4 & view_matrix, const glm::vec3 camera_position) const
{
	LoadValue(location_viewMatrix, view_matrix);
	LoadValue(location_cameraPosition, camera_position);
}

void CWaterShader::LoadTransformMatrix(const glm::mat4 & modelMatrix) const
{
	LoadValue(location_TransformMatrix, modelMatrix);
}

void CWaterShader::LoadWaterColor(const glm::vec4 & water_color) const
{
	LoadValue(location_WaterColor, water_color);
}
