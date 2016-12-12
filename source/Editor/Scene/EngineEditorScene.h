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
//class CGameEditorScene : public CScene
//{
//	shared_ptr<CEntity> sphere_plane;
//	shared_ptr<CPlayer<CKnight>> player = nullptr;
//	shared_ptr<CEntity>  lightsphere;
//	shared_ptr<CEntity> bob;
//	vector<shared_ptr<CKnight>> knights;
//
//	int m_CameraType = 0;
//	float time_x = 0;
//	float camera_or_distance;
//public:
//	CGUICreator m_GUICreator;
//	bool thridCamera = true;
//	CGameEditorScene(CGame* game, int camera_type)
//		: CScene(game)
//		, m_CameraType(camera_type)
//		, m_GUICreator(game->GetInputManager())
//	{
//		m_Name = "Test Scene";
//		m_SceneFile = "Data/Maps/TestMap.map";
//	}
//	~CGameEditorScene()
//	{
//		std::cout << "Destructor " << typeid(*this).name() << endl;
//	}
//	int Initialize() override
//	{
//		m_TerrainsPtr.resize(m_TerrainsCount*m_TerrainsCount);
//		for (unsigned int z = 0; z < m_TerrainsCount*m_TerrainsCount; z++)
//			m_Terrains.push_back(make_shared<CTerrain>(m_Loader));
//		int ti = 0;
//		//	for (unsigned int z = 0; z < m_TerrainsCount; z++)
//		//	for (unsigned int x = 0; x < m_TerrainsCount; x++)
//		uint x = -1;
//		uint z = 0;
//
//		for (auto& t : m_Terrains)
//		{
//
//			x++;
//			if (x >= m_TerrainsCount)
//			{
//				x = 0;
//				z++;
//			}
//
//			CTerrain* current_terrain = dynamic_cast<CTerrain*>(t.get());
//			if (current_terrain == nullptr)
//				continue;
//
//			m_TerrainsPtr[ti++] = current_terrain;
//			string name;
//			name += "Terrain" + to_string(x) + "x" + to_string(z);
//
//			//CreateNewEmptyTerrain(name, x, z);
//			string height_map = "Data/Terrain/HeightMaps/terrain_" + name + ".terrain";// +name + ".terrain";
//			string blend_map = "Data/Terrain/BlendMaps/terrain_" + name + ".png";
//			//CreateEmptyHeightMap(height_map, 32, 32);
//			try
//			{
//				current_terrain->Init(name,
//					(float)x, (float)z,
//					height_map, blend_map,
//					"Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png", "Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png",
//					"Data/Textures/165.png", "Data/Textures/165.png",
//					"Data/Textures/grassFlowers.png", "Data/Textures/grassFlowers.png",
//					"Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png", "Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png",
//					"Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png", "Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png"
//				);
//
//				string grass_file = "Data/Terrain/GrassPositions/terrain_" + name + ".terrain";
//				//current_terrain.GenerateGrassPositions(grass_file, 10000, 1.0f);
//				current_terrain->InitGrassFromFile(grass_file, m_Loader.LoadTexture("Data/Textures/G3_Nature_Plant_Grass_06_Diffuse_01.png"));
//
//				for (int x = 1; x <= 6; x++)
//				{
//					uint tree1 = m_Loader.LoadMesh("Data/Meshes/woodland_pack_1/WOODLAND_PACK/WOODLAND_TREES/f_tree" + to_string(x) + "/obj__tree" + to_string(x) + ".obj", false, false);
//
//					//CAssimModel tree1(m_Loader.GetTextureLoader());
//					//string ftree_name = "Data/Meshes/woodland_pack_1/WOODLAND_PACK/WOODLAND_TREES/f_tree" + to_string(x) + "/obj__tree" + to_string(x) + ".obj";
//					try
//					{
//						//tree1.InitModel(ftree_name);
//						//tree1.m_UseFakeLight = true;
//						current_terrain->m_Trees.push_back(tree1);
//					}
//					catch (std::runtime_error& e)
//					{
//						cout << e.what() << endl;
//					}
//
//				}
//
//				string trees_file = "Data/Terrain/TreesPositions/terrain_" + name + ".terrain";
//				//current_terrain.GenerateTreePositions(trees_file, 1000, 7.f, current_terrain.m_Trees.size(), 2);
//				current_terrain->InitTreesFromFile(trees_file, glm::vec3(0, 10, 0));
//
//				//Flora
//				//CAssimModel fern(m_Loader.GetTextureLoader());
//				uint fernn = m_Loader.LoadMesh("Data/Meshes/Fern1/Fern.obj", false, false);
//				current_terrain->m_Trees.push_back(fernn);
//
//				string flora_file = "Data/Terrain/Flora/terrain_" + name + ".terrain";
//				current_terrain->GenerateTreePositions(flora_file, 1000, 10.f, 1, 0.1f, 6);
//				current_terrain->InitTreesFromFile(flora_file, glm::vec3(0, 1, 0), 6);
//			}
//			catch (const std::exception& e)
//			{
//				cout << e.what();
//			}
//
//
//		}
//
//		player = make_shared<CPlayer<CKnight>>(&m_Game->GetInputManager(), CreatePositionVector(100, 120));
//		player->Create(this);
//		AddGameObject(player, true);
//
//
//		//std::this_thread::sleep_for(std::chrono::seconds(10));
//
//		//shared_ptr<CKnight> knight = make_shared<CKnight>();
//		//	knight->Create(this);
//		//	knight->SetPosition(CreatePositionVector(100, 130));
//		//	AddEntity(knight, true);
//		//	knights.push_back(knight);
//
//		//for (int y = 0; y < 1; y++)
//		//	for (int x = 0; x < 1; x++)
//		//	{//Data/Meshes/Garen/garen_idle.fbx
//		//		//"Data/Meshes/Bob/boblampclean.md5mesh"
//		//		bob = CreateEntityFromFile("Data/Meshes/Garen/garen_idle.fbx", ColiderType::NONE_COLIDER, glm::vec3(0, 0, 1.5));
//		//		bob->SetPosition(CreatePositionVector(x+100, 130 - y));
//		//		bob->SetRotation(glm::vec3(180, 0, 180));
//		//		m_Loader.m_Models[bob->GetModelId()]->m_TimeUpdate = true;
//		//		m_Loader.m_Models[bob->GetModelId()]->m_BoneUpdate = true;
//		//		m_Loader.m_Models[bob->GetModelId()]->SetTime(x / 100);
//		//		AddEntity(bob, true);
//		//	}
//
//
//		/*	for (int y = 0; y < 2; y++)
//		for (int x = 0; x < 10; x++)
//		{
//		shared_ptr<CKnight> knight = make_shared<CKnight>();
//		knight->Create(this);
//		knight->SetPosition(CreatePositionVector(101.f+(float)x, 120.f+ (float)y));
//		AddEntity(knight, true);
//		knights.push_back(knight);
//		}*/
//
//
//		if (m_CameraType == 0)
//			setThridCamera();
//		else
//			setFirstCamera();
//		camera_or_distance = m_Camera->GetDistance();
//
//		int water_dudv_id = m_Loader.LoadTexture("Data/Textures/waterDUDV.png");
//		int water_normal_id = m_Loader.LoadTexture("Data/Textures/waternormal.png");
//
//		shared_ptr<CTextInfo> water_dudv = m_Loader.m_Textures[water_dudv_id];
//		shared_ptr<CTextInfo> water_normal = m_Loader.m_Textures[water_normal_id];
//
//		//	shared_ptr<CGameObject> w = std::make_shared<CWaterTile>(glm::vec3(250, -0.1, 110), 100.f, 0.1f, water_dudv.get(), water_normal.get(), glm::vec4(43.0f / 255.f, 106.f / 255.f, 134.f / 255.f, 0.2));
//		//	AddGameObject(w);
//		//	m_Loader.AddObjectToOpenGLLoadingPass(&m_WaterTiles.back());
//		//	
//		//CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.12, 0.05));
//		//m_Gui.guiButtons.push_back(testButton);
//
//
//		////CGUITexture(GLuint textureID, glm::vec2 position, glm::vec2 scale);
//		////CGUITexture testframe(m_Loader.LoadTexture("Data/GUI/Package1/Inventory.png"), glm::vec2(0.5, 00.0), glm::vec2(0.45, 1.0));
//		//CGUITexture testframe(m_Loader.LoadTexture("Data/GUI/Package1/frame.png"), glm::vec2(0.0, 00.0), glm::vec2(0.8));
//		//m_Gui.guiTextures.push_back(testframe);
//
//		//CGUIText(string text, glm::vec2 position, float font_size, glm::vec3 colour);
//
//		//m_Game.GetGUIRenderer().AddTexture(m_Loader.LoadTexture("Data/GUI/Package1/Inventory.png").GetId(), glm::vec2(0.5, 00.0), glm::vec2(0.45, 1.0));
//		//unsigned int characters_frame_id = m_Game.GetGUIRenderer().AddTexture(m_Loader.LoadTexture("Data/GUI/Package1/frame.png").GetId(), glm::vec2(0.0, 00.0), glm::vec2(0.8));
//
//
//		/*m_Game->m_SceneParser.LoadGui("Data/GUI/testgui.xml", this, m_Game->GetGUIRenderer());
//
//		m_GUICreator.SetWindowSize(m_Game->GetDisplayManager().GetWindowSize());
//		m_GUICreator.SetElementToEdit(m_Game->GetGUIRenderer().GetElement("Stats"));
//		*/
//
//
//
//		/*m_Game.GetGUIRenderer().AddText("Strength : 10\nAgility: 10\nVitality: 10\nEnergy: 10", glm::vec2(-0.6495, 0.295), 0.75, Utils::RGBtoFloat(glm::vec3(0, 0, 0)));
//		m_Game.GetGUIRenderer().AddText("Strength : 10\nAgility: 10\nVitality: 10\nEnergy: 10", glm::vec2(-0.65, 0.3), 0.75, Utils::RGBtoFloat(glm::vec3(117, 95, 61)));
//		m_Game.GetGUIRenderer().AddText("Character Stats", glm::vec2(-0.1495, 0.495), 1, Utils::RGBtoFloat(glm::vec3(0, 0, 0)));
//		m_Game.GetGUIRenderer().AddText("Character Stats", glm::vec2(-0.15, 0.5), 1, Utils::RGBtoFloat(glm::vec3(117, 95, 61)));
//		*/
//		/*	CGUITexture iframe(m_Loader.LoadTexture("Data/GUI/Package1/inventory.png"), glm::vec2(0.5, 00.0), glm::vec2(0.65, 0.8));
//		m_Gui.guiTextures.push_back(iframe);*/
//		m_MousePicker = CMousePicker(m_Camera, m_Game->GetDisplayManager().GetWindowSize(), m_Game->GetProjectionMatrix());
//		m_MousePicker.SetTerrainsPtr(&m_TerrainsPtr, m_TerrainsCount);
//		time_x = 0;
//
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
//	void setThridCamera()
//	{
//		m_Camera.reset();
//		if (player != nullptr)
//			m_Camera = make_shared<CThirdPersonCamera>(&m_Game->GetInputManager(), player->GetReferencedPosition(), player->GetReferencedRotation());
//		else
//			m_Camera = make_shared<CThirdPersonCamera>(&m_Game->GetInputManager(), Utils::s_vec3_zero, Utils::s_vec3_zero);
//		m_Camera->SetProjectionMatrix(m_Game->GetProjectionMatrix());
//	}
//
//	int Update() override
//	{
//		float dt = static_cast<float>(m_Game->GetDisplayManager().GetDeltaTime());
//
//		m_GUICreator.UpdateGuiElementPosition();
//
//		m_GloabalTime += dt;
//		m_DayNightCycle.Update(dt);
//
//		if (m_Game->GetInputManager().GetKey(KeyCodes::P))
//		{
//			for (const auto& entity : m_PhysicsEntities)
//			{
//				entity->m_RigidBody.m_AngularVelocity = glm::vec3(0, 0, 0);
//			}
//		}
//		if (m_Game->GetInputManager().GetKey(KeyCodes::G))
//		{
//			if (m_Game->GetGUIRenderer() != nullptr)
//				m_Game->m_SceneParser.SaveGUI("Data/GUI/testgui.xml", *m_Game->GetGUIRenderer().get());
//		}
//		if (m_Game->GetInputManager().GetKey(KeyCodes::T))
//		{
//			/*	shared_ptr<CEntity> sphere = CreateEntityFromFile("Data/Meshes/Sphere/sphere.obj", ColiderType::SPHERE_COLIDER , glm::vec3(0), false, CreatePositionVector(200 + static_cast<float>(rand() % 10 - 5)*5, 350 + static_cast<float>(rand()%10 - 5)*5, 100), glm::vec3(0), glm::vec3(5));
//			sphere->m_IsSpecial = true;
//			sphere->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 5));
//			g_pages_mutex.lock();
//			AddEntity(sphere, true);
//			m_PhysicsEntities.push_back(sphere.get());
//			m_PhysicsScene.AddRigidBody(&sphere->m_RigidBody);
//			g_pages_mutex.unlock();*/
//		}
//
//		if (m_Game->GetDisplayManager().CheckActiveWindow() && !m_Game->GetInputManager().GetKey(KeyCodes::LCTRL))
//		{
//			m_Camera->CalculateInput();
//			if (m_CameraType == 0)
//				m_Game->GetDisplayManager().ShowCoursor(false);
//		}
//		else
//		{
//			if (m_CameraType == 0)
//				m_Game->GetDisplayManager().ShowCoursor(true);
//		}
//		m_Camera->Move();
//
//		if (m_Game->GetInputManager().GetKey(KeyCodes::M))
//		{
//			MergeAllTerrains();
//		}
//
//		if (m_Game->GetInputManager().GetKey(KeyCodes::Q))
//		{
//			glm::vec3 point;
//			CTerrain* terr = m_MousePicker.GetMousePointOnTerrain(point, m_Game->GetInputManager().GetMousePosition());
//			if (terr != nullptr)
//			{
//				for (int y = terr->GetPosition().z - 1; y < terr->GetPosition().z + 2; y++)
//					for (int x = terr->GetPosition().x - 1; x < terr->GetPosition().x + 2; x++)
//					{
//						if (y < 0 || x < 0 || y > m_TerrainsCount || x > m_TerrainsCount)
//							continue;
//						CTerrain* terrain = GetTerrain(x, y);
//						if (terrain == nullptr)
//							continue;
//						terrain->PaintHeightMapPoint(glm::vec2(point.x, point.z), m_BrushSize, m_HeightPaintStrength, m_ApplyLimits, m_HeightUpperLimit, m_HeightDownLimit);
//					}
//			}
//		}
//
//
//		if (m_Game->GetInputManager().GetKey(KeyCodes::E))
//		{
//			glm::vec3 point;
//			CTerrain* terrain = m_MousePicker.GetMousePointOnTerrain(point, m_Game->GetInputManager().GetMousePosition());
//			if (terrain != nullptr)
//			{
//				for (int y = terrain->GetPosition().z - 1; y < terrain->GetPosition().z + 2; y++)
//					for (int x = terrain->GetPosition().x - 1; x < terrain->GetPosition().x + 2; x++)
//					{
//						if (y < 0 || x < 0 || y > m_TerrainsCount || x > m_TerrainsCount)
//							continue;
//						terrain->PaintBlendMap(point, m_BrushSize, m_PaintColor);
//					}
//			}
//		}
//		if (player != nullptr)
//			player->Move(dt);
//
//		for (const auto& knight : knights)
//		{
//			knight->Move(dt);
//		}
//
//		glm::vec2 window_size = m_Game->GetDisplayManager().GetWindowSize();
//
//		//if (m_Gui.guiButtons[0].CheckStatus(window_size) == GuiButtonState::ACTIVE)
//		//{
//		////	m_Loader.ReloadTexture("Data/Terrain/TerrainTextures/blendMap.png", m_Terrains[0].m_BackgroundTexture[0]);
//		//	
//		//}
//		m_Loader.UpdateModels(static_cast<float>(dt));
//
//		//LockCameraUnderTerrain();
//
//		return 0;
//	}
//
//	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
//};
//
//
//
