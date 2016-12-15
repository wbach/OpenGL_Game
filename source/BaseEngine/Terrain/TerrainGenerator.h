#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include "../Utils/Utils.h"
#include "PerlinNoise.h"
class CTerrainGenerator
{
public:
	void CreateNewEmptyTerrain(const std::string& name, const float& x, const float& z, const int& sx, const int& sy, bool procedural = true, const float& terrain_size = 100.f);	
	float CalculateRoughness(const float& base_roughness, const int& vertex_size, const float& terrain_size);
private:
	float GetProceduralHeight(const float& x, const float& y) const;
	float RandFloat(const int& v) const;
	float GetNoise(const int& x, const int& y) const;
	float GetSmoothNoise(const int& x, const int& z) const;
	float GetInterpolatedNoise(const float& x, const float& z) const;

	float Interpolate(const float& a, const float& b, const float& blend)  const;
	void CreateEmptyHeightMap(std::string filename, int xx, int yy, bool procedural = true, const int& world_x = 0, const int& world_z = 0) const;

	int m_Octaves = 3;
	float m_Roughness = 0.6f ;
	float m_Amplitude = 35.2f;
	PerlinNoise pnoise;
};