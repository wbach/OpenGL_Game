//#pragma once
//#include <string>
//#include <vector>
//#include "../BaseEngine/Engine/Scene.h"
//#include "../BaseEngine/Camera/ThridPersonCamera.h"
//#include "../BaseEngine/Camera/FirstPersonCamera.h"
//#include "../BaseEngine/GUI/Buttons/GUIButton.h"
//#include "../RPGpack/Player.h"
//#include "../BaseEngine/Engine/GLgame.h"
//#include "../RPGpack/HeroClass/Knight.h"
//#include "../BaseEngine/GUI/Creator/GUICreator.h"
//
//class CSimpleScene : public CScene
//{
//public:
//	CSimpleScene(CGame* game)
//		: CScene(game)		
//	{
//		m_Name = "Simple Scene";
//		m_SceneFile = "Data/Maps/Simple.map";
//		m_TerrainsCount = 0;
//	}
//	~CSimpleScene()
//	{
//	}
//	int Initialize() override
//	{
//		m_TerrainsPtr.resize(m_TerrainsCount*m_TerrainsCount);
//		for (unsigned int z = 0; z < m_TerrainsCount*m_TerrainsCount; z++)
//			m_Terrains.push_back(CTerrain(m_Loader));
//		int ti = 0;
//		uint x = 0;
//		uint z = 0;
//
//		for (auto& current_terrain : m_Terrains)
//		{
//			m_TerrainsPtr[ti++] = current_terrain.get();
//			string name;
//			name += "Terrain" + to_string(x) + "x" + to_string(z);
//			string height_map = "Data/Terrain/HeightMaps/terrain_" + name + ".terrain";// +name + ".terrain";
//			string blend_map = "Data/Terrain/BlendMaps/terrain_" + name + ".png";
//			try
//			{
//				current_terrain.Init(name,
//					(float)x, (float)z,
//					height_map, blend_map,
//					"Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png", "Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png",
//					"Data/Textures/165.png", "Data/Textures/165.png",
//					"Data/Textures/grassFlowers.png", "Data/Textures/grassFlowers.png",
//					"Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png", "Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png",
//					"Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png", "Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png"
//				);
//			}
//			catch (const std::exception& e)
//			{
//				cout << e.what();
//			}
//			x++;
//			if (x >= m_TerrainsCount)
//			{
//				x = 0;
//				z++;
//			}
//		}
//		setFirstCamera();
//		m_DayNightCycle.SetTime(0.5f);
//
//		return 0;
//	}
//	virtual void PostInitialize() override
//	{
//		//	m_Loader.CreateUpdateThread();
//	}
//	void setFirstCamera()
//	{
//		m_Camera.reset();
//		m_Camera = make_shared<CFirstPersonCamera>(&m_Game->GetInputManager(), &m_Game->GetDisplayManager());
//		m_Camera->SetPitch(5.6f);
//		m_Camera->SetYaw(94.0f);
//		m_Camera->SetProjectionMatrix(m_Game->GetProjectionMatrix());
//	}
//	int Update() override
//	{
//		float dt = static_cast<float>(m_Game->GetDisplayManager().GetDeltaTime());
//
//		m_GloabalTime += dt;
//		m_DayNightCycle.Update(dt);
//		
//		if (m_Game->GetDisplayManager().CheckActiveWindow() && !m_Game->GetInputManager().GetKey(KeyCodes::LCTRL))
//		{
//			m_Camera->CalculateInput();
//		}
//		else
//		{
//		}
//		m_Camera->Move();		
//		m_Loader.UpdateModels(static_cast<float>(dt));
//		return 0;
//	}
//
//	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
//};