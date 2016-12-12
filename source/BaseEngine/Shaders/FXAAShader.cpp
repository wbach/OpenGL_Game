#include "FXAAShader.h"

void CFXAAShader::Init()
{
	InitShaderProgram("Data/Shaders/PostProcess/FXAA.vert", "Data/Shaders/PostProcess/FXAA.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CFXAAShader::LoadFxaaSpanMax(const float &v) const
{
	LoadValue(location_FxaaSpanMax, v);
}
void CFXAAShader::LoadFxaaReduceMin(const float &v) const
{
	LoadValue(location_FxaaReduceMin, v);
}
void CFXAAShader::LoadFxaaReduceMul(const float &v) const
{
	LoadValue(location_FxaaReduceMul, v);
}
void CFXAAShader::LoadScreenSize(const glm::vec2& screen_size) const
{
	LoadValue(location_ScreenSize, screen_size);
}
void CFXAAShader::ConnectTextureUnits() const
{
	LoadValue(location_Texture, 0);
}
void CFXAAShader::GetAllUniformLocations()
{
	location_ScreenSize = GetUniformLocation("ScreenSize");
	location_FxaaSpanMax = GetUniformLocation("FxaaSpanMax");
	location_FxaaReduceMin = GetUniformLocation("FxaaReduceMin");
	location_FxaaReduceMul = GetUniformLocation("FxaaReduceMul");
	location_Texture = GetUniformLocation("FilterTexture");
}
void CFXAAShader::BindAttributes()
{
	BindAttribute(0, "position");
}
