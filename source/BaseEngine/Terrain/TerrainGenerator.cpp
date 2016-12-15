#include "TerrainGenerator.h"

void CTerrainGenerator::CreateNewEmptyTerrain(const std::string & name, const float & x, const float & z, const int & sx, const int & sy, bool procedural, const float & terrain_size)
{
	std::string height_map = "Data/Terrain/HeightMaps/terrain_" + name + ".terrain";// +name + ".terrain";
	m_Roughness = CalculateRoughness(m_Roughness, sx, terrain_size);
	CreateEmptyHeightMap(height_map, sx, sy, procedural, x, z);
	std::string blend_map = "Data/Terrain/BlendMaps/terrain_" + name + ".png";
}

float CTerrainGenerator::CalculateRoughness(const float & base_roughness, const int & vertex_size, const float & terrain_size)
{
	float x = vertex_size / 32.f;
	float y = terrain_size / 100.f;
	return  base_roughness / x / y;
}

float CTerrainGenerator::GetProceduralHeight(const float & x, const float & y) const
{
//	return 1;
	return GetInterpolatedNoise(x, y) * m_Amplitude;

	float total = 0;
	float d = (float)pow(2, m_Octaves - 1);
	for (int i = 0; i< m_Octaves; i++) {
		float freq = 1.f / 512.f;// (float)(pow(2, i) / d);
		float amp = (float)pow(m_Roughness, i) * m_Amplitude;
		total += GetInterpolatedNoise((x)*freq, (y)*freq) * amp;
	}
	return total;
}

float CTerrainGenerator::RandFloat(const int & v) const
{
	return ((float)(rand() % v) / (float)v) - .5f;
}

float CTerrainGenerator::GetNoise(const int & x, const int & y) const
{
	return pnoise.noise(x, y, 0.8f);
}

float CTerrainGenerator::GetSmoothNoise(const int & x, const int & z) const
{
	float corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1)
		+ GetNoise(x + 1, z + 1)) / 16.f;
	float sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1)
		+ GetNoise(x, z + 1)) / 8.f;
	float center = GetNoise(x, z) / 4.f;
	return corners + sides + center;
}

float CTerrainGenerator::GetInterpolatedNoise(const float & x, const float & z) const
{
	int intX = (int)x;
	int intZ = (int)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = GetSmoothNoise(intX, intZ);
	float v2 = GetSmoothNoise(intX + 1, intZ);
	float v3 = GetSmoothNoise(intX, intZ + 1);
	float v4 = GetSmoothNoise(intX + 1, intZ + 1);
	float i1 = Interpolate(v1, v2, fracX);
	float i2 = Interpolate(v3, v4, fracX);
	return Interpolate(i1, i2, fracZ);
}

float CTerrainGenerator::Interpolate(const float & a, const float & b, const float & blend) const
{
	double theta = blend * M_PI;
	float f = (float)(1.f - cos(theta)) * 0.5f;
	return a * (1.f - f) + b * f;
}

void CTerrainGenerator::CreateEmptyHeightMap(std::string filename, int xx, int yy, bool procedural, const int & world_x, const int & world_z) const
{
	std::ofstream file;
	file.open(filename);
	float xstep = m_Roughness;
	float ystep = m_Roughness;
	file << "r" << xx << "x" << yy << std::endl;
	float yoffset = 0;
	for (int y = 0; y < yy; y++)
	{
		if (y == yy - 1)
			yoffset += ystep;
		float xoffset = 0;
		for (int x = 0; x < xx; x++)
		{
			if (x == xx - 1)
				xoffset += xstep;
			float h = procedural ? GetProceduralHeight(xoffset + (xstep*world_x) * xx, yoffset + (ystep*world_z) * yy) : 0.f;
			h -= (m_Amplitude / 2.f);
			//float h = (sign_1[x] + sign_2[y] ) / 2.f;
			file << h << " ";
			xoffset += xstep;
		}
		file << std::endl;
		yoffset += ystep;
	}
	file.close();
}
