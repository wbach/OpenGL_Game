#include "GUIShader.h"

void GUIShader::Init()
{
	InitShaderProgram("Data/Shaders/Gui/guiVertexShader.vert" ,"Data/Shaders/Gui/guiFragmentShader.frag" ) ;
	Start() ;
	GetAllUniformLocations();
	Stop();
}

void GUIShader::GetAllUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
}

void GUIShader::BindAttributes() 
{
	BindAttribute(0, "position");
}

void GUIShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_transformationMatrix, matrix);
}

GUIShader::~GUIShader()
{

}
