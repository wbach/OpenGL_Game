#pragma once
#include <string>
#include <vector>
#include "../../BaseEngine/Engine/Scene.h"
#include "../../BaseEngine/Camera/ThridPersonCamera.h"
#include "../HeroClass/Knight.h"
#include "../../RPGpack/GUI/CRPGGUIManager.h"
#include "../Player.h"

class CRPGMainScene : public CScene
{
public:
	CRPGMainScene(CGame* game);
	~CRPGMainScene();
	void LockCameraUnderTerrain();
	int Initialize() override;
	void PostInitialize() override;
	int Update() override;
	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
private:
	CPlayer player;
	float camera_or_distance;
};