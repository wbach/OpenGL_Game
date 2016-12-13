#pragma once
#include "../Physics/PhysicsScene.h"
#include "../Controllers/MousePicker.h"
#include "../Entities/Entity.h"
#include "../Loader/Loader.h"
#include "../Lights/Light.h"
#include "../Camera/Camera.h"
#include "../Terrain/Terrain.h"
#include "../GUI/GUI.h"
#include "../Game/DayNightCycle.h"
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "../Water/WaterTile.h"
#include <list>
using namespace std;

class CGame;
class CScene
{
public:
	CScene(CGame* game);
	void AddSkyBoxTexture(int type, std::string texture);
	void AddSkyBoxCubeTexture(int type, vector<string> texture);
	void LoadSkyBox();

	void AddLight(CLight light) { m_Lights.push_back(light); }

	void AddGameObject(shared_ptr<CGameObject> game_object, bool direct = false, bool interacted = false);

	void SaveHeightMaps();
	void SaveBlendMaps();
	void MergeTerrains(CTerrain& t1, CTerrain& t2, int axis);
	void MergeAllTerrains();
	CTerrain* FindTerrainByName(string name);

	void ApplyPhysicsToObjects(float dt);
	void ClearObjectsVelocity();

	void DeleteEntity(shared_ptr<CEntity>& entity);
	bool DeleteSubEntity(shared_ptr<CEntity>& entity, int id);
	shared_ptr<CEntity> CreateEntityFromFile(string file_name, const ColiderType::Type type = ColiderType::NONE_COLIDER, glm::vec3 normalized_size = glm::vec3(0, 1, 0), bool instanced = false,
											glm::vec3 pos = glm::vec3(10, 10, 0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1, 1, 1),
											const glm::mat4& parent_matrix = glm::mat4(1.f)
											);
	shared_ptr<CEntity> CreateAnimEntityFromFile(string file_name, const ColiderType::Type type = ColiderType::NONE_COLIDER, glm::vec3 normalized_size = glm::vec3(0, 1, 0), bool instanced = false,
		glm::vec3 pos = glm::vec3(10, 10, 0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1, 1, 1),
		const glm::mat4& parent_matrix = glm::mat4(1.f)
	);

	
	void GetGameObjectsRecursive(CGameObject* game_object);
	

	std::list<CEntity*> GetPhysicsEntitiesInRange(const glm::vec3& position);
	vector<CEntity*>&	GetPhysicsEntities() { return m_PhysicsEntities; }
	CGameObject* FindGameObject(const uint& id);
	shared_ptr<CEntity> FindSubEntity(shared_ptr<CEntity>& entity, int id);
	CTerrain* FindTerrainById(int id);
		
	CGameObjectsList&					GetGameObjects();
	CGameObjectsList&					GetTerrains() ;
	CTerrain*							GetTerrain(int x, int y);
	const vector<CLight>&				GetLights() const;

	std::list<CWaterTile*>  GetWaterTilesInCameraRange();
	std::list<CEntity*>  GetEntitiesInCameraRange();
	std::list<CTerrain*> GetTerrainsInCameraRange();
	void CheckTerrainInCameraRange();
	void CheckObjectsInCameraRange();
	void CheckObjectType(CGameObject* go);

	void LoadTerrainsFloora(string file_name);

	const string& GetName();

	virtual int		Initialize() = 0;
	virtual void	PostInitialize() {};
	virtual int		Update() = 0;

	const virtual glm::mat4& GetViewMatrix() = 0;

	// create position vector (x, heightTerrain(x,z),z)
	glm::vec3 CreatePositionVector(float x, float z, float y_offset = 0);
	glm::vec3 CreatePositionVector(glm::vec2 xzPos, float y_offset = 0);
	
	const float GetHeightOfWorld(float x, float y,float z);
	//return height form current terrain
	const float GetHeightOfTerrain(float x, float z);
	const float GetHeightOfTerrain(glm::vec2 xz_pos);
	void TerrainNumber(float world_x, float world_z, int&x, int& z);
	void TerrainNumber(glm::vec2 xz_pos, int&x, int& z);

	CLoader& GetLoader() { return m_Loader; }
	shared_ptr<CGUI>& GetGUI() { return m_GUI; }

	const glm::vec3			GetCameraPosition() const { return m_Camera->GetPosition(); }
	shared_ptr<CCamera>&	GetCamera() { return m_Camera; }
	
	CGame* GetGamePtr() { return m_Game; }

	//First of all , day-night cycle
	void SetDirectionalLightColour(const glm::vec3& color);
	const CLight& GetDirectionalLight() const;
	const glm::vec3& GetDirectionalLightPosition() const;

	const vector<string>& GetSkyBoxDayTextures() const { return m_DaySkyboxTextures; }
	const vector<string>& GetSkyBoxNightTextures() const { return m_NightSkyboxTextures; }
	CMousePicker& GetMousePicker() { return m_MousePicker; }
	glm::vec3 GetMousePickerTarget();
	void SetEntityToMousePointByKey(std::shared_ptr<CEntity>& entity);

	//Remove only loaded by parser elements
	bool m_IsDebug = false;
	std::string m_SceneFile;

	//ForPainting
	CTerrain*	m_CurrentTerrain;

	CPhysicsScene m_PhysicsScene;
	float		  m_PhysicsDistance = 20.f;

	float m_MaxEntityViewDistane = 200.f;
	std::mutex g_pages_mutex;

	
	//std::list<CTerrain> m_Terrains;

	unsigned int m_TerrainsCount;
	unsigned int m_TerrainViewRadius;

	float m_HeightPaintStrength = 0.1f;
	bool m_ApplyLimits = false;
	float m_HeightUpperLimit = 100.f;
	float m_HeightDownLimit = 0.f;

	glm::vec3 m_PaintColor = glm::vec3(0);
	int m_BrushSize = 4;

	float m_GloabalTime = 0.f;

	CDayNightCycle m_DayNightCycle;

	void Reset();
	~CScene();
protected:
	CMousePicker m_MousePicker;
	CLoader m_Loader;
	string	m_Name;
	CGame*	m_Game;

	shared_ptr<CGUI> m_GUI;

	//Minimum one light on scene only (night - decrease strength)
	CLight				m_DirectionalLight;
	vector<CLight>		m_Lights;
	shared_ptr<CCamera> m_Camera;
	//vector<CWaterTile>	m_WaterTiles;		
	vector<CEntity*>	m_PhysicsEntities;

	//vector<shared_ptr<CEntity>> m_Entities;
	
	CGameObjectsList	m_Terrains;
	CGameObjectsList	m_GameObjects;
	CGameObjectsPtrList m_InteractedObjects; //global

	friend class CXmlSceneParser;

	vector<string> m_DaySkyboxTextures;
	vector<string> m_NightSkyboxTextures;

	CGameObjectsPtrList		m_InteractedEntitiesInCameraRange;
	std::list<CWaterTile*>	m_WaterTilesInCameraRange;
	std::list<CEntity*>		m_EntitiesInCameraRange;
	std::list<CTerrain*>	m_TerrainInCameraRange;
	std::vector<CTerrain*>  m_TerrainsPtr; // All 
};



