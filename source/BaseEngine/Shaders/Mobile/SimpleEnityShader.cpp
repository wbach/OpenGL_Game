#include "SimpleEnityShader.h"

void CSimpleEntityShader::Init()
{
	InitShaderProgram("Data/Shaders/Simple/SimpleEntityShader.vert", "Data/Shaders/Simple/SimpleEntityShader.frag");
	Start();
	GetAllUniformLocations();
	Stop();
}

void CSimpleEntityShader::GetAllUniformLocations()
{
	location_TransformationMatrix = GetUniformLocation("TransformationMatrix");
	location_ProjectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_ViewMatrix = GetUniformLocation("ViewMatrix");

	location_IsInstancedRender = GetUniformLocation("IsInstancedRender");

	//Animations
	location_UseBoneTransform = GetUniformLocation("UseBoneTransform");

	for (int x = 0; x < MAX_BONES; x++)
	{
		char tmpVariableName[50]; ; memset(tmpVariableName, 0, 50);
		sprintf(tmpVariableName, "Bones[%i]", x);
		location_Bones[x] = GetUniformLocation(tmpVariableName);
	}
}

void CSimpleEntityShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(4, "TransformationMatrixes");
}
void CSimpleEntityShader::LoadUseInstancedRendering(const float & use) const
{
	LoadValue(location_IsInstancedRender, use);
}
void CSimpleEntityShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_TransformationMatrix, matrix);
}

void CSimpleEntityShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ProjectionMatrix, matrix);
}

void CSimpleEntityShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ViewMatrix, matrix);
}
void CSimpleEntityShader::LoadUseBonesTransformation(const float & is) const
{
	LoadValue(location_UseBoneTransform, is);
}
void CSimpleEntityShader::LoadBoneTransform(const glm::mat4 & transform, unsigned int id) const
{
	LoadValue(location_Bones[id], transform);
}