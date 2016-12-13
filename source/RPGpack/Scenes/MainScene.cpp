#include "MainScene.h"
#include "../../BaseEngine/Engine/GLgame.h"

CRPGMainScene::CRPGMainScene(CGame* game)
	: CScene(game)
	, player(&game->GetInputManager())
{
	m_Name = "RPG Main Scene";
	m_SceneFile = "Data/Maps/MainRPGMap.map";
}

CRPGMainScene::~CRPGMainScene()
{
}


int CRPGMainScene::Initialize()
{
	m_TerrainsPtr.resize(m_TerrainsCount*m_TerrainsCount);
	for (unsigned int z = 0; z < m_TerrainsCount*m_TerrainsCount; z++)
		m_Terrains.push_back(make_shared<CTerrain>(m_Loader));
	int ti = 0;
	uint x = -1;
	uint z = 0;

	for (auto& t : m_Terrains)
	{
		x++;
		if (x >= m_TerrainsCount)
		{
			x = 0;
			z++;
		}
		CTerrain* current_terrain = dynamic_cast<CTerrain*>(t.get());
		if (current_terrain == nullptr)
			continue;

		m_TerrainsPtr[ti++] = current_terrain;
		string name;
		name += "Terrain" + to_string(x) + "x" + to_string(z);

		//CreateNewEmptyTerrain(name, x, z);
		string height_map = "Data/Terrain/HeightMaps/terrain_" + name + ".terrain";// +name + ".terrain";
		string blend_map = "Data/Terrain/BlendMaps/terrain_" + name + ".png";
		//CreateEmptyHeightMap(height_map, 32, 32);
		try
		{
			current_terrain->Init(name,
				(float)x, (float)z,
				height_map, blend_map,
				"Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png", "Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png",
				"Data/Textures/G3_Architecture_Rock_01_Diffuse_01.png", "Data/Textures/G3_Architecture_Rock_01_Diffuse_01.png",
				"Data/Textures/grassFlowers.png", "Data/Textures/grassFlowers.png",
				"Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png", "Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png",
				"Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png", "Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png"
			);
			string grass_file = "Data/Terrain/GrassPositions/terrain_" + name + ".terrain";
			//current_terrain.GenerateGrassPositions(grass_file, 10000, 1.0f);
			current_terrain->InitGrassFromFile(grass_file, m_Loader.LoadTexture("Data/Textures/G3_Nature_Plant_Grass_06_Diffuse_01.png"));

			for (int x = 1; x <= 6; x++)
			{
				uint tree1 = m_Loader.LoadMesh("Data/Meshes/woodland_pack_1/WOODLAND_PACK/WOODLAND_TREES/f_tree" + to_string(x) + "/obj__tree" + to_string(x) + ".obj", false, false);
				try
				{
					current_terrain->m_Trees.push_back(tree1);
				}
				catch (std::runtime_error& e)
				{
					cout << e.what() << endl;
				}
			}
			string trees_file = "Data/Terrain/TreesPositions/terrain_" + name + ".terrain";
			//current_terrain.GenerateTreePositions(trees_file, 1000, 7.f, current_terrain.m_Trees.size(), 2);
			current_terrain->InitTreesFromFile(trees_file, glm::vec3(0, 10, 0));

			//Flora
			uint fernn = m_Loader.LoadMesh("Data/Meshes/Fern1/Fern.obj", false, false);
			current_terrain->m_Trees.push_back(fernn);

			string flora_file = "Data/Terrain/Flora/terrain_" + name + ".terrain";
			current_terrain->GenerateTreePositions(flora_file, 1000, 10.f, 1, 0.1f, 6);
			current_terrain->InitTreesFromFile(flora_file, glm::vec3(0, 1, 0), 6);
		}
		catch (const std::exception& e)
		{
			cout << e.what();
		}
	}


	// Create a Player
	player.SetHero(make_shared<CKnight>());
	player.SetPosition(CreatePositionVector(100, 120));
	player.GetHero()->Create(this);
	AddGameObject(player.GetHero(), true);

	shared_ptr<CEntity> shop = CreateAnimEntityFromFile("Data/Meshes/Shop/shopMale_idle.fbx", ColiderType::SPHERE_COLIDER, glm::vec3(0, 1.58, 0), false, CreatePositionVector(102, 125), glm::vec3(0, 180, 0));
	AddGameObject(shop, true, false);

	shared_ptr<CEntity> vayne = CreateAnimEntityFromFile("Data/Meshes/Vayne/Vayne_idle.fbx", ColiderType::SPHERE_COLIDER, glm::vec3(0, 1.8, 0), false, CreatePositionVector(100, 125), glm::vec3(0, 180, 0));
	uint mid = vayne->GetModelId();
	GetLoader().m_Models[mid]->m_UseFakeLight = true;
	AddGameObject(vayne, true, false);

	shared_ptr<CEntity> alistar = CreateAnimEntityFromFile("Data/Meshes/Alistar/Alistar_idle.fbx", ColiderType::SPHERE_COLIDER, glm::vec3(0, 2.8, 0), false, CreatePositionVector(98, 125), glm::vec3(0, 180, 0));
	AddGameObject(alistar, true, true);

//	shared_ptr<CEntity> fwar = CreateAnimEntityFromFile("Data/Meshes/Female warriors/Femalewarriors.fbx", ColiderType::NONE_COLIDER, glm::vec3(0, 1.8, 0), false, CreatePositionVector(96, 125), glm::vec3(0, 180, 0));
//	AddGameObject(fwar, true);

	int water_dudv_id = m_Loader.LoadTexture("Data/Textures/waterDUDV.png");
	int water_normal_id = m_Loader.LoadTexture("Data/Textures/waternormal.png");
	shared_ptr<CTextInfo> water_dudv = m_Loader.m_Textures[water_dudv_id];
	shared_ptr<CTextInfo> water_normal = m_Loader.m_Textures[water_normal_id];
	shared_ptr<CGameObject> w = std::make_shared<CWaterTile>(glm::vec3(250, -0.1, 110), 100.f, 0.1f, water_dudv.get(), water_normal.get(), glm::vec4(43.0f / 255.f, 106.f / 255.f, 134.f / 255.f, 0.2));
	AddGameObject(w, true);
	COpenGLObject* wo = dynamic_cast<COpenGLObject*>(w.get());
	if(wo !=nullptr)
		m_Loader.AddObjectToOpenGLLoadingPass(wo);

	//Create Camera
	if (player.GetHero() != nullptr)
		m_Camera = make_shared<CThirdPersonCamera>(&m_Game->GetInputManager(), player.GetHero()->GetReferencedPosition(), player.GetHero()->GetReferencedRotation());
	else
		m_Camera = make_shared<CThirdPersonCamera>(&m_Game->GetInputManager(), Utils::s_vec3_zero, Utils::s_vec3_zero);

	m_Camera->SetProjectionMatrix(m_Game->GetProjectionMatrix());

	m_DayNightCycle.SetTime(0.5f);

	m_Game->GetDisplayManager().ShowCoursor(false);

	m_GUI = make_shared<CRPGGUIManager>(m_Game->GetInputManager());

	if (m_GUI != nullptr)
	{
		m_Game->GetGUIRenderer().InitGUI(*m_GUI);

		m_Game->m_SceneParser.LoadGui("Data/GUI/RpgGUI.xml", this);

		m_GUI->AssignActions();
	}		
	return 0;
}

void CRPGMainScene::PostInitialize()
{
}

int CRPGMainScene::Update()
{
	float dt = static_cast<float>(m_Game->GetDisplayManager().GetDeltaTime());

	if (m_Camera != nullptr)
	{
		m_Camera->CalculateInput();
		m_Camera->Move();
	}

	m_GloabalTime += dt;
	m_DayNightCycle.Update(dt);

	player.Move(dt);

	m_Loader.UpdateModels(static_cast<float>(dt));

	if (m_GUI != nullptr)
	{
		m_GUI->GUIActions();
		CRPGGUIManager* gui = dynamic_cast<CRPGGUIManager*>(m_GUI.get());
		if (gui != nullptr)
		{
			gui->Update(player.GetHero().get());
			gui->RenderHerosBars(m_Game->GetProjectionMatrix(), m_Camera->GetViewMatrix(), m_InteractedEntitiesInCameraRange);
		}		
	}
		

	return 0;
}


void CRPGMainScene::LockCameraUnderTerrain()
{
	float camera_r = 1.f;

	m_Camera->SetDistance(camera_or_distance);
	m_Camera->Move();
	glm::vec3 orginal_pos = m_Camera->GetPosition();
	glm::vec3 orginal_dir = m_Camera->GetDirection();
	CRigidbody crb(SSphere(m_Camera->GetPosition(), camera_r));

	float camera_distance = m_Camera->GetDistance();

	std::list<CRigidbody> bodys;
	float camera_min_distance = .25f;
	for (auto& entity : m_PhysicsEntities)
	{
		for (auto& f : entity->m_RigidBody.m_Colider.GetFaces())
		{
			float x = Utils::IntersectTriangle(f.normal, orginal_pos, orginal_dir, f.vertexes);
			float y = Utils::IntersectTriangle(-f.normal, orginal_pos, orginal_dir, f.vertexes);
			if (x > 0 && x < camera_or_distance + 0.001f)
			{
				camera_distance -= x;
				if (camera_distance < camera_min_distance)
					camera_distance = camera_min_distance;
				m_Camera->SetDistance(camera_distance);
				m_Camera->Move();
			}
			if (y > 0 && y < camera_or_distance + 0.001f)
			{
				camera_distance -= y;
				if (camera_distance < camera_min_distance)
					camera_distance = camera_min_distance;
				m_Camera->SetDistance(camera_distance);
				m_Camera->Move();
			}

		}
		bodys.push_back(entity->m_RigidBody);
	}

	glm::vec2 camera_position = glm::vec2(m_Camera->GetPosition().x, m_Camera->GetPosition().z);
	int x, z;
	TerrainNumber(camera_position, x, z);
	if (x >= 0 && z >= 0)
	{
		CTerrain* terrain = GetTerrain(x, z);
		if (terrain == nullptr)
			return;
		float height = terrain->GetHeightofTerrain(camera_position.x, camera_position.y);
		if (m_Camera->GetPosition().y > height) return;
		m_Camera->SetPosition(glm::vec3(camera_position.x, height + .1f, camera_position.y));
		m_Camera->UpdateViewMatrix();
	}
}