#pragma once
#include "../Entities/Entity.h"
#include "../Loader/Material.h"
#include "../Loader/EmptyLoader.h"
#include "../Loader/Loader.h"
#include "../Engine/Transform.h"
#include "../Utils/Utils.h"
#include "../Engine/GameObject.h"
#include <math.h>
#include "glm/glm.hpp"
#include "Grass.h"

static const float MAX_PIXEL_COLOUR = 256.0f * 256.0f * 256.0f;
static const float TERRAIN_SIZE = 100.f;
static const int IndicesGridWidth = 500;
static const int IndicesGridHeight = 500;

struct SPaintInfo
{
	glm::vec2 Point;
	float value;
};

class CTerrain : public CGameObject
{	
public:	
	CTerrain(CLoader &loader);
	~CTerrain();

	bool m_IsInit = false;

	void Init(string name, 
		float x, float z,
		string height_map, string blend_map,		
		string background_texture, string background_normal_texture, 
		string rock_texture, string rock_normal_texture,
		string r_texture, string r_normal_texture, 
		string g_texture, string g_normal_texture,
		string b_texture, string b_normal_texture);
	void InitHeights(int x, int y);
	void InitGrassFromFile(std::string filename, const int& texture);
	void InitTreesFromFile(std::string filename, glm::vec3 normalized_size, const int& offset = 0);

	void CreateTerrainVertexes(int x_start, int y_start, int width, int height);
	void CreateTerrain();
	glm::vec3 CalculateNormalMap(int x, int z);

	void GenerateTerrainMap(CLoader &loader, string heightMap);
	vector<glm::vec3> GenerateGrassPositions(const string& filename, const int& count, const float& min_distance) const;
	vector<glm::vec3> GenerateTreePositions(const string& filename, const int& count, const float& min_distance, const int& trees, const float& size, const int& seleced_nr = -1) const;

	void AddTerrainGrass(const std::vector<glm::vec3>& positions, const int& texture);	
	
		  float		GetHeightMap(int x, int z);
	const float		GetHeightofTerrain(glm::vec2 posXZ) const;
	const float		GetHeightofTerrain(float worldX, float worldZ) const;
	const glm::vec3 GetNormalOfTerrain(glm::vec2 posXZ) const;
	const glm::vec3		GetNormalOfTerrain(float worldX, float worldZ) const;
	const float&	GetHeight(int x, int y) const;
	const glm::vec3 GetNormal(int x, int y) const;
	const string	GetName() const;
	const string	GetNameWithId() const;
	const glm::vec3 GetPosition() const;	
	const float&	GetSize() const;
	const int&		GetId() { return m_Id; }

	void SetName(std::string name);	
	void SetHeight(int x, int y, float value);	

	void PaintBlendMap(glm::vec3 point, int m_BrushSize, glm::vec3 paint_color);
	void PaintHeightMapPoint(glm::vec2 point, int brush_size, float strength, bool apply_limits, float up_limit, float down_limit);
	void ReloadVertex();

	vector<uint> m_Trees;

	//To painting
	GLubyte* m_BlendMapData = nullptr;
	int m_BlendMapWidth;
	int m_BlendMapHeight;	

	//Terrain height paint
	int m_HeightMapResolution;	
	float m_AvarageHeight;	

	// Grass and other flora elements - flowers...
	std::list<CGrass> m_Grass;

	CLoader&	 m_Loader;
	CEmptyLoader m_Model;

	//World position
	STransform m_Transform;
	glm::vec3 m_WorldCenterPosition;

	int m_BlendMap;
	int m_BackgroundTexture[2];
	int m_RTexture[2];
	int m_GTexture[2];
	int m_BTexture[2];
	int m_RockTexture[2];
	std::string m_HeightMapPath, m_BlendMapPath, m_BackgorungTexturePath[2], m_RTexturePath[2], m_BTexturePath[2], m_GTexturePath[2], m_RockTexturePath[2];	
private:
	std::vector<unsigned short> m_Indices;
	std::vector<float> m_Vertices;
	std::vector<float> m_Normals;
	std::vector<float> m_Tangens;
	std::vector<float> m_TextureCoords;

	std::vector<float> m_Heights;

	// x , z (0,0 .. 1,0)
	glm::vec3 m_Position;
	string m_Name;

	int m_Id;
	static int s_ID;
};
