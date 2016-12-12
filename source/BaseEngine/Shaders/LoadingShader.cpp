#include "LoadingShader.h"

void LoadingShader::Init()
{
	InitShaderProgram("Data/Shaders/Game/loadingShader.vert", "Data/Shaders/Game/loadingShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void LoadingShader::GetAllUniformLocations() 
{
	location_isTextured = GetUniformLocation("isTextured");
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_modelTexture = GetUniformLocation("modelTexture");
	location_alpha = GetUniformLocation("alphaBlend");
}
void LoadingShader::LoadIsTextured(const float& is_tex) const 
{
	LoadValue(location_isTextured, is_tex);

}
void LoadingShader::ConnectTextureUnits() const
{
	LoadValue(location_modelTexture, 0);
}
void LoadingShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
}
void LoadingShader::LoadTransformMatrix(const glm::mat4& matrix) const 
{
	LoadValue(location_transformationMatrix, matrix);
}
void LoadingShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_projectionMatrix, matrix);
}
void LoadingShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_viewMatrix, matrix);
}
void LoadingShader::LoadAlphaValue(const float& alpha) const
{
	LoadValue(location_alpha, alpha);
}
LoadingShader::~LoadingShader() 
{
}
