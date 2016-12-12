#pragma once
#include <string>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "Texture.h"
using namespace std;
struct SMaterial
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shineDamper;
	float reflectivity;
	vector<int> textures;
	bool isTransparency;
	bool useFakeLighting;
	int numberOfRows;
	int textureIndex;

	SMaterial()
	{
		textureIndex = 0;
		numberOfRows = 1;
		useFakeLighting = false;
		isTransparency = false;
		ambient = specular = glm::vec3(0);
		diffuse = glm::vec3(0.8);
	}
	float getTextureXOffset() 
	{
		int column = textureIndex%numberOfRows;
		return (float)column / (float)numberOfRows;
	}
	float getTextureYOffset()
	{
		int row = textureIndex / numberOfRows;
		return (float)row / (float)numberOfRows;
	}
};
