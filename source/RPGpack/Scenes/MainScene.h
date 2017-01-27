#pragma once
#include <string>
#include <vector>
#include "../../BaseEngine/Engine/Scene.h"
#include "../../BaseEngine/Camera/ThridPersonCamera.h"
#include "../HeroClass/Knight.h"
#include "../../RPGpack/GUI/CRPGGUIManager.h"
#include "../Player.h"
#include "../../BaseEngine/Terrain/TerrainGenerator.h"
#include <mutex>
class CRPGMainScene : public CScene
{
public:
	CRPGMainScene(CGame* game);
	~CRPGMainScene();
	void LockCameraUnderTerrain();
	int Initialize() override;
	void PostInitialize() override;
	int Update() override;
	void BackgroungLoadingThread();
	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }

	void AddTerrainToLoad(const float& x, const float& z);

	CGameObject* GetObjectLoad();
	void AddObjectToLoad(CGameObject* obj);
private:
	CPlayer player;
	float camera_or_distance;
	std::list<CGameObject*> m_BackgroundLoadingObjects;
	std::mutex m_MutexO;
};