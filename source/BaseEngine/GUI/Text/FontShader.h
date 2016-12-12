#pragma once
#include "../../Shaders/ShaderProgram.h"
#include "../../Lights/Light.h"
#include "../../Utils/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../../Camera/Camera.h"

class FontShader : public CShaderProgram
{
public:
	void Init(){
		InitShaderProgram("Data/Shaders/Font/fontVertex.vert" ,"Data/Shaders/Font/fontFragment.frag" ) ;
		Start() ;
		GetAllUniformLocations();
		glm::mat4 scaleMat = Utils::CreateTransformationMatrix(glm::vec2(0),glm::vec2(0.0005,0.001));
		loadTransformation(scaleMat);
		Stop();
	}
	void GetAllUniformLocations() override
	{
		location_translation = GetUniformLocation("translation");
		location_transformationMatrix = GetUniformLocation("transformationMatrix");
	}
	void BindAttributes() override 
	{

	}
	void loadColour(const glm::vec3& colour) const
	{
	}
	void loadTranslation(const glm::vec2& pos) const
	{
		LoadValue(location_translation, pos);
	}
	void loadTransformation(const glm::mat4& pos) const
	{
		LoadValue(location_transformationMatrix, pos);
	}
private:
	int location_translation;
	int location_transformationMatrix;
};
