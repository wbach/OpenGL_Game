#include "XMLSceneParser.h"


bool CXmlSceneParser::ParaseBolean(rapidxml::xml_node<>* node)
{
	if (std::stof(std::string(node->value())) != 0)
		return true;
	return false;
}

glm::vec3 CXmlSceneParser::ParseVector3d(rapidxml::xml_node<>* node)
{
	glm::vec3 vector;
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("x").compare(subnode->name()))
			vector.x = std::stof(subnode->value());
		else if (!std::string("y").compare(subnode->name()))
			vector.y = std::stof(subnode->value());
		else if (!std::string("z").compare(subnode->name()))
			vector.z = std::stof(subnode->value());
	}
	return vector;
}

std::vector<std::string> CXmlSceneParser::ParaseCubeTextures(rapidxml::xml_node<>* node)
{
	std::string strings[6];

	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("RightTexture").compare(subnode->name()))
			strings[0] = subnode->value();

		if (!std::string("LeftTexture").compare(subnode->name()))
			strings[1] = subnode->value();

		if (!std::string("TopTexture").compare(subnode->name()))
			strings[2] = subnode->value();

		if (!std::string("BottomTexture").compare(subnode->name()))
			strings[3] = subnode->value();

		if (!std::string("BackTexture").compare(subnode->name()))
			strings[4] = subnode->value();

		if (!std::string("FrontTexture").compare(subnode->name()))
			strings[5] = subnode->value();
	}
	std::vector<std::string> textures;
	for (std::string& string : strings)
		textures.push_back(string);

	return textures;
}

void CXmlSceneParser::ParseCamera(rapidxml::xml_node<>* node)
{
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Position").compare(subnode->name()))
			m_Scene->GetCamera()->SetPosition(ParseVector3d(subnode));

		if (!std::string("Pitch").compare(subnode->name()))
			m_Scene->GetCamera()->SetPitch(std::stof(std::string(subnode->value())));

		if (!std::string("Yaw").compare(subnode->name()))
			m_Scene->GetCamera()->SetYaw(std::stof(std::string(subnode->value())));
	}
}

void CXmlSceneParser::ParseSkyBox(rapidxml::xml_node<>* node)
{
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("DayTextures").compare(subnode->name()))
			m_Scene->AddSkyBoxCubeTexture(0, ParaseCubeTextures(subnode));

		if (!std::string("NightTextures").compare(subnode->name()))
			m_Scene->AddSkyBoxCubeTexture(1, ParaseCubeTextures(subnode));
	}
	m_Scene->LoadSkyBox();
}

void CXmlSceneParser::ParseLight(rapidxml::xml_node<>* node)
{
	shared_ptr<CEntity> entity = nullptr;
	glm::vec3 position, color, attentionn;
	int type;
	float cutOff;
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Type").compare(subnode->name()))
			type = std::stoi(std::string(subnode->value()));

		if (!std::string("Position").compare(subnode->name()))
			position = ParseVector3d(subnode);

		if (!std::string("Color").compare(subnode->name()))
			color = ParseVector3d(subnode);

		if (!std::string("Attenuation").compare(subnode->name()))
			attentionn = ParseVector3d(subnode);

		if (!std::string("CutOff").compare(subnode->name()))
			cutOff = std::stof(std::string(subnode->value()));
	}
	//CLight(glm::vec3 position, glm::vec3 colour, glm::vec3 attenuation, float cutOff);
	CLight light(position, color, attentionn, cutOff);
	light.SetType(type);

	m_Scene->AddLight(light);

	
}

void CXmlSceneParser::ParseTerrain(rapidxml::xml_node<>* node)
{
	std::string name, height_map, blend_map, backgorung_texture[2], r_texture[2], b_texture[2], g_texture[2], rock_texture[2];
	glm::vec3 position;
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Name").compare(subnode->name()))
			name = subnode->value();

		if (!std::string("Position").compare(subnode->name()))
			position = ParseVector3d(subnode);

		if (!std::string("HeightMap").compare(subnode->name()))
			height_map = subnode->value();

		if (!std::string("BlendMap").compare(subnode->name()))
			blend_map = subnode->value();

		if (!std::string("BackgroundTexture").compare(subnode->name()))
			ParseTexture(subnode, backgorung_texture[0], backgorung_texture[1]);

		if (!std::string("RockTexture").compare(subnode->name()))
			ParseTexture(subnode, rock_texture[0], rock_texture[1]);

		if (!std::string("RedTexture").compare(subnode->name()))
			ParseTexture(subnode, r_texture[0], r_texture[1]);

		if (!std::string("GreenTexture").compare(subnode->name()))
			ParseTexture(subnode, g_texture[0], g_texture[1]);

		if (!std::string("BlueTexture").compare(subnode->name()))
			ParseTexture(subnode, b_texture[0], b_texture[1]);
	}

	//m_Scene->AddTerrain(CTerrain(name, m_Scene->m_Loader,
	//	position.x, position.z, 
	//	height_map, blend_map,
	//	backgorung_texture[0], backgorung_texture[1],
	//	rock_texture[0], rock_texture[1],
	//	r_texture[0], r_texture[1],
	//	g_texture[0], g_texture[1],
	//	b_texture[0], b_texture[1]
	//));
}

void CXmlSceneParser::ParseTexture(rapidxml::xml_node<>* node, std::string& diff_texture, std::string& normal_texture)
{
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("DiffTexture").compare(subnode->name()))
			diff_texture = subnode->value();

		if (!std::string("NormalTexture").compare(subnode->name()))
			normal_texture = subnode->value();
	}
}

void CXmlSceneParser::ParaseEntity(rapidxml::xml_node<>* node, CEntity* parent)
{
	glm::vec3 normalized_size(0), position(0), rotation(0), scale(0);
	bool is_global(false), children_culling(false);
	std::string filename, name;
	int colider(0);
	float attach_offset(0);

	std::list<rapidxml::xml_node<>*> children_nodes;

	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{	
		if (!std::string("File").compare(subnode->name()))
			filename = Utils::ConvertToRelativePath(subnode->value());	

		if (!std::string("NormalizedSize").compare(subnode->name()))
			normalized_size = ParseVector3d(subnode);

		if (!std::string("ChildrenCulling").compare(subnode->name()))
			children_culling = std::stoi(subnode->value()) != 0 ? true : false;		

		if (!std::string("Name").compare(subnode->name()))
			name = subnode->value();		

		if (!std::string("Position").compare(subnode->name()))
			position = ParseVector3d(subnode);			

		if (!std::string("AttachYOffset").compare(subnode->name()))
			attach_offset = std::stof(std::string(subnode->value()));

		if (!std::string("Rotation").compare(subnode->name()))
			rotation = ParseVector3d(subnode);		

		if (!std::string("Scale").compare(subnode->name()))
			scale = ParseVector3d(subnode);			

		if (!std::string("ColiderType").compare(subnode->name()))
			colider = std::stoi(subnode->value());			

		if (!std::string("Global").compare(subnode->name()))
			is_global = ParaseBolean(subnode);

		if (!std::string("Entity").compare(subnode->name()))
			children_nodes.push_back(subnode);
	}
	shared_ptr<CEntity> entity = nullptr;
	glm::mat4 parent_matrix(1.f);
	if (parent != nullptr)
		parent_matrix = parent->GetTransformMatrix();
	entity = m_Scene->CreateEntityFromFile(filename, static_cast<ColiderType::Type>(colider), normalized_size, false, position, rotation, scale, parent_matrix);

	if (entity == nullptr)
		return;

	entity->SetIsCullingChildren(children_culling);
	entity->GetAttachYOffset() = attach_offset;	

	for (rapidxml::xml_node<>* n : children_nodes)
	{
		ParaseEntity(n, entity.get());
	}			

	entity->CalculateTransformMatrix();
	if (parent == nullptr)	
		m_Scene->AddGameObject(entity, is_global);
	else
	{
		parent->AddSubbObject(entity);
		//entity->m_RigidBody.m_Colider.TransformFaces(entity->GetTransformMatrix());
	}
}

void CXmlSceneParser::ParaseGuiTexture(rapidxml::xml_node<>* node, string& name, string & file, glm::vec2 & pos, glm::vec2 & size)
{
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Name").compare(subnode->name()))
		{
			name = subnode->value();
		}
		if (!std::string("File").compare(subnode->name()))
		{
			file = subnode->value();
		}
		if (!std::string("Position").compare(subnode->name()))
		{
			glm::vec3 p = ParseVector3d(subnode);
			pos = glm::vec2(p.x, p.y);
		}
		if (!std::string("Size").compare(subnode->name()))
		{
			glm::vec3 p = ParseVector3d(subnode);
			size = glm::vec2(p.x, p.y);
		}
	}
}

void CXmlSceneParser::ParaseGuiText(rapidxml::xml_node<>* node, string& name, string & text, glm::vec2 & pos, glm::vec3 & color, float & font_size)
{
	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Name").compare(subnode->name()))
		{
			name = subnode->value();
		}
		if (!std::string("Text").compare(subnode->name()))
		{
			text = subnode->value();
		}
		if (!std::string("Position").compare(subnode->name()))
		{
			glm::vec3 p = ParseVector3d(subnode);
			pos = glm::vec2(p.x, p.y);
		}
		if (!std::string("Color").compare(subnode->name()))
		{
			color = ParseVector3d(subnode);
		}
		if (!std::string("FontSize").compare(subnode->name()))
		{
			font_size = stof(subnode->value());
		}
	}
}

void CXmlSceneParser::ParaseGuiWindow(rapidxml::xml_node<>* node, CScene* scene)
{
	shared_ptr<CGUIWindow> gui_window = make_shared<CGUIWindow>();

	for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
	{
		if (!std::string("Name").compare(subnode->name()))
		{
			gui_window->SetName(subnode->value());
		}
		if (!std::string("Position").compare(subnode->name()))
		{
			glm::vec3 pos = ParseVector3d(subnode);
			gui_window->SetPosition(glm::vec2(pos.x, pos.y));
		}
		if (!std::string("Size").compare(subnode->name()))
		{
			glm::vec3 s = ParseVector3d(subnode);
			gui_window->SetSize(glm::vec2(s.x, s.y));
		}
		if (!std::string("GUITexture").compare(subnode->name()))
		{
			string file, name;
			glm::vec2 pos, size;
			ParaseGuiTexture(subnode, name, file, pos, size);
			uint tx = scene->GetLoader().LoadTexture(file);
			shared_ptr<CGUITexture> t = make_shared<CGUITexture>(scene->GetLoader().m_Textures[tx].get(), pos + gui_window->GetPosition(), size);
			t->SetName(name);
			gui_window->AddSubElement(t);
		//	unsigned int id =  guirenderer.AddTexture(name, gui_window, scene->GetLoader().LoadTexture(file), pos + gui_window.GetPosition(), size);
		}
			
		if (!std::string("GUIText").compare(subnode->name()))
		{
			string text, name;
			glm::vec2 pos; float size;
			glm::vec3 color;
			ParaseGuiText(subnode, name, text, pos , color, size);
			shared_ptr<CGUIText> t = make_shared<CGUIText>(text, pos + gui_window->GetPosition(), size, color);
			t->SetName(name);
			gui_window->AddSubElement(t);
		//	unsigned int id = guirenderer.AddText(name, gui_window, text, pos + gui_window.GetPosition(), size, color);
		}
	}
	scene->GetGUI()->AddElement(gui_window);
}

void CXmlSceneParser::LoadGui(std::string file_name, CScene* scene)
{
	if (scene->GetGUI() == nullptr)
		return;

	std::string all_file = GetStringFile(file_name);

	if (all_file.empty())
		return;

	rapidxml::xml_document <> document;
	try
	{
		document.parse<0>(const_cast<char*>(all_file.c_str()));
	}
	catch (rapidxml::parse_error p)
	{
		p.what();
	}

	rapidxml::xml_node <>* prefab = document.first_node();

	for (rapidxml::xml_node <>* node = prefab->first_node(); node; node = node->next_sibling())
	{
		if (!std::string("GUIWindow").compare(node->name()))
			ParaseGuiWindow(node, scene);
	}
	scene->GetGUI()->SetElements();
	document.clear();
}

void CXmlSceneParser::SaveGUI(std::string file_name, CScene* scene)
{
	if (scene->GetGUI() == nullptr)
		return;
	rapidxml::xml_document <> dokument;

	rapidxml::xml_node <>* map = dokument.allocate_node(rapidxml::node_declaration);
	map->append_attribute(dokument.allocate_attribute("version", "1.0"));
	map->append_attribute(dokument.allocate_attribute("encoding", "UTF-8"));
	dokument.append_node(map);

	rapidxml::xml_node<>* root = dokument.allocate_node(rapidxml::node_element, "GUI");
	dokument.append_node(root);

	for (const auto& e : scene->GetGUI()->GetAllElements())
	{
		if(e->m_SaveToFile)
			e->Save(dokument, root);
	}
	
	std::ofstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open : " << file_name << "." << std::endl;
		return;
	}

	file << dokument;

	file.close();

	dokument.clear();
}

void CXmlSceneParser::LoadScene(std::string file_name, CScene* scene, void(*func)(int p))
{
	int percent = 0;
	if (func != nullptr) func(percent);

	m_Scene = scene;

	std::string all_file = GetStringFile(file_name);

	if (all_file.empty()) return;

	rapidxml::xml_document <> dokument;
	try
	{
		dokument.parse<0>(const_cast<char*>(all_file.c_str()));
	}
	catch (rapidxml::parse_error p)
	{
		p.what();
		return;
	}
	percent = 5;
	if (func != nullptr) func(percent);

	rapidxml::xml_node <>* map = dokument.first_node();
	for (rapidxml::xml_node <>* node = map->first_node(); node; node = node->next_sibling())
	{
		if (!std::string("Camera").compare(node->name()))
		{
			ParseCamera(node);
			percent += 5;
			if (func != nullptr) func(percent);
		}
		if (!std::string("Entity").compare(node->name()))
		{
			ParaseEntity(node);
			percent += 35;
			if (func != nullptr) func(percent);
		}
		if (!std::string("Terrain").compare(node->name()))
		{
			//ParseTerrain(node);
			percent += 35;
			if (func != nullptr) func(percent);
		}
		if (!std::string("Light").compare(node->name()))
		{
			ParseLight(node);
			percent += 5;
			if (func != nullptr) func(percent);
		}
		if (!std::string("SkyBox").compare(node->name()))
		{
			ParseSkyBox(node);
			percent += 5;
			if (func != nullptr) func(percent);
		}
		if (!std::string("Prefab").compare(node->name()))
		{
			for (rapidxml::xml_node<>* subnode = node->first_node(); subnode; subnode = subnode->next_sibling())
			{
				if (!std::string("File").compare(subnode->name()))
					LoadPrefab(subnode->value(), scene);
			}	
			percent += 10;
			if (func != nullptr) func(percent);
		}
	}
	dokument.clear();
}

void CXmlSceneParser::SaveToFile(std::string file_name, CScene* scene)
{
	rapidxml::xml_document <> dokument;

	rapidxml::xml_node <>* map = dokument.allocate_node(rapidxml::node_declaration);
	map->append_attribute(dokument.allocate_attribute("version", "1.0"));
	map->append_attribute(dokument.allocate_attribute("encoding", "UTF-8"));
	dokument.append_node(map);

	rapidxml::xml_node<>* root = dokument.allocate_node(rapidxml::node_element, "Map");
	dokument.append_node(root);


	AddCameraNode(dokument, root, scene->GetCamera());
	AddSkyBoxNode(dokument, root, scene->GetSkyBoxDayTextures(), scene->GetSkyBoxNightTextures());

	for (const CLight& light : m_Scene->GetLights())
	{
		AddLightNode(dokument, root, light);
	}

	for (const auto& go : scene->GetGameObjects())
	{				
		CEntity* entity = dynamic_cast<CEntity*>(go.get());
		AddEntityNode(dokument, root, entity, 1);
	}

	for (const auto& go : scene->GetTerrains())
	{
		CTerrain* terrain = dynamic_cast<CTerrain*>(go.get());
		if (terrain == nullptr)
			return;

		for (const auto& go : terrain->GetSubGameObjects())
		{
			CEntity* entity = dynamic_cast<CEntity*>(go.get());
			AddEntityNode(dokument, root, entity, 1);
		}
	}

	std::ofstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open map: " << file_name << "." << std::endl;
		return;
	}

	file << dokument;

	file.close();

	dokument.clear();
}
void CXmlSceneParser::LoadPrefab(std::string file_name, CScene* scene, rapidxml::xml_node<>* parent)
{
	std::string all_file = GetStringFile(file_name);

	rapidxml::xml_document <> document;
	try
	{
		document.parse<0>(const_cast<char*>(all_file.c_str()));
	}
	catch (rapidxml::parse_error p)
	{
		p.what();
	}

	rapidxml::xml_node <>* prefab = document.first_node();
	for (rapidxml::xml_node <>* node = prefab->first_node(); node; node = node->next_sibling())
	{
		if (!std::string("Entity").compare(node->name()))
			ParaseEntity(node);
	}
	if (parent != nullptr)
	{
		parent->append_node(prefab);
	}
	document.clear();

}
std::string CXmlSceneParser::GetStringFile(const std::string & file_name)
{
	std::string line;
	std::string all_file;
	std::ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open file: " << file_name << "." << std::endl;
		return all_file;
	}
	while (std::getline(file, line))
	{
		all_file += line;
	}
	file.close();

	return all_file;
}
void CXmlSceneParser::AddVectorToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const glm::vec3 vector)
{
	rapidxml::xml_node<>* x = document.allocate_node(rapidxml::node_element, "x", document.allocate_string(std::to_string(vector.x).c_str()));
	rapidxml::xml_node<>* y = document.allocate_node(rapidxml::node_element, "y", document.allocate_string((std::to_string(vector.y).c_str())));
	rapidxml::xml_node<>* z = document.allocate_node(rapidxml::node_element, "z", document.allocate_string((std::to_string(vector.z).c_str())));
	node->append_node(x);
	node->append_node(y);
	node->append_node(z);
}

void CXmlSceneParser::AddCubeTextureToNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, const vector<string>& textures)
{
	if (textures.size() < 6)
		return;
	rapidxml::xml_node<>* r = document.allocate_node(rapidxml::node_element, "RightTexture", document.allocate_string(textures[0].c_str()));
	rapidxml::xml_node<>* l = document.allocate_node(rapidxml::node_element, "LeftTexture", document.allocate_string(textures[1].c_str()));
	rapidxml::xml_node<>* t = document.allocate_node(rapidxml::node_element, "TopTexture", document.allocate_string(textures[2].c_str()));
	rapidxml::xml_node<>* b = document.allocate_node(rapidxml::node_element, "BottomTexture", document.allocate_string(textures[3].c_str()));
	rapidxml::xml_node<>* d = document.allocate_node(rapidxml::node_element, "BackTexture", document.allocate_string(textures[4].c_str()));
	rapidxml::xml_node<>* f = document.allocate_node(rapidxml::node_element, "FrontTexture", document.allocate_string(textures[5].c_str()));
	node->append_node(r);
	node->append_node(l);
	node->append_node(t);
	node->append_node(b);
	node->append_node(d);
	node->append_node(f);
}

void CXmlSceneParser::AddEntityNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, CEntity* entity, int global)
{
	//Enity created in code not i
	if (entity->IsSpecial()) return;

	rapidxml::xml_node<>* entity_node = document.allocate_node(rapidxml::node_element, "Entity");

	rapidxml::xml_node<>* file = document.allocate_node(rapidxml::node_element, "File", document.allocate_string(entity->GetFullPath().c_str()));


	rapidxml::xml_node<>* name = document.allocate_node(rapidxml::node_element, "Name", document.allocate_string(entity->GetName().c_str()));

	rapidxml::xml_node<>* children_culling = document.allocate_node(rapidxml::node_element, "ChildrenCulling ", document.allocate_string(std::to_string(entity->GetIsCullingChildren()).c_str()));

	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	AddVectorToNode(document, position, entity->GetLocalPosition());

	rapidxml::xml_node<>* attach_y_offset = document.allocate_node(rapidxml::node_element, "AttachYOffset", document.allocate_string(std::to_string(entity->GetAttachYOffset()).c_str()));


	rapidxml::xml_node<>* rotation = document.allocate_node(rapidxml::node_element, "Rotation");
	AddVectorToNode(document, rotation, entity->GetRotation());

	rapidxml::xml_node<>* scale = document.allocate_node(rapidxml::node_element, "Scale");
	AddVectorToNode(document, scale, entity->GetScale());

	rapidxml::xml_node<>* normalized = document.allocate_node(rapidxml::node_element, "NormalizedSize");
	AddVectorToNode(document, normalized, entity->GetNormalizedSize());	

	rapidxml::xml_node<>* colider_type = document.allocate_node(rapidxml::node_element, "ColiderType", "2");

	rapidxml::xml_node<>* _global = document.allocate_node(rapidxml::node_element, "Global", document.allocate_string(std::to_string(global).c_str()));
	
	entity_node->append_node(file);
	entity_node->append_node(name); 
	entity_node->append_node(children_culling);
	entity_node->append_node(position);
	entity_node->append_node(attach_y_offset);
	entity_node->append_node(rotation);
	entity_node->append_node(scale);
	entity_node->append_node(normalized);
	entity_node->append_node(colider_type);
	entity_node->append_node(_global);

	for (const shared_ptr<CGameObject>& child : entity->GetSubGameObjects())
	{
	//	AddEntityNode(document, entity_node, child, global);
	}

	node->append_node(entity_node);
}

void CXmlSceneParser::AddTerrainNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, const CTerrain& terrain)
{
	rapidxml::xml_node<>* terrain_node = document.allocate_node(rapidxml::node_element, "Terrain");

	rapidxml::xml_node<>* name_node = document.allocate_node(rapidxml::node_element, "Name", document.allocate_string(terrain.GetName().c_str()));
	terrain_node->append_node(name_node);
	
	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	AddVectorToNode(document, position, terrain.GetPosition());
	terrain_node->append_node(position);

	rapidxml::xml_node<>* height_map = document.allocate_node(rapidxml::node_element, "HeightMap", document.allocate_string(terrain.m_HeightMapPath.c_str()));
	terrain_node->append_node(height_map);

	rapidxml::xml_node<>* blend_map = document.allocate_node(rapidxml::node_element, "BlendMap", document.allocate_string(terrain.m_BlendMapPath.c_str()));
	terrain_node->append_node(blend_map);

	rapidxml::xml_node<>* background = document.allocate_node(rapidxml::node_element, "BackgroundTexture");
	rapidxml::xml_node<>* background_diff = document.allocate_node(rapidxml::node_element, "DiffTexture", document.allocate_string(terrain.m_BackgorungTexturePath[0].c_str()));
	rapidxml::xml_node<>* background_normal = document.allocate_node(rapidxml::node_element, "NormalTexture", document.allocate_string(terrain.m_BackgorungTexturePath[1].c_str()));
	background->append_node(background_diff);
	background->append_node(background_normal);
	terrain_node->append_node(background);

	rapidxml::xml_node<>* rockground = document.allocate_node(rapidxml::node_element, "RockTexture");
	rapidxml::xml_node<>* rock_diff = document.allocate_node(rapidxml::node_element, "DiffTexture", document.allocate_string(terrain.m_RockTexturePath[0].c_str()));
	rapidxml::xml_node<>* rock_normal = document.allocate_node(rapidxml::node_element, "NormalTexture", document.allocate_string(terrain.m_RockTexturePath[1].c_str()));
	rockground->append_node(rock_diff);
	rockground->append_node(rock_normal);
	terrain_node->append_node(rockground);

	rapidxml::xml_node<>* r_texure = document.allocate_node(rapidxml::node_element, "RedTexture");
	rapidxml::xml_node<>* r_texure_diff = document.allocate_node(rapidxml::node_element, "DiffTexture", document.allocate_string(terrain.m_RTexturePath[0].c_str()));
	rapidxml::xml_node<>* r_texure_normal = document.allocate_node(rapidxml::node_element, "NormalTexture", document.allocate_string(terrain.m_RTexturePath[1].c_str()));
	r_texure->append_node(r_texure_diff);
	r_texure->append_node(r_texure_normal);
	terrain_node->append_node(r_texure);

	rapidxml::xml_node<>* g_texture = document.allocate_node(rapidxml::node_element, "GreenTexture");
	rapidxml::xml_node<>* g_texture_diff = document.allocate_node(rapidxml::node_element, "DiffTexture", document.allocate_string(terrain.m_GTexturePath[0].c_str()));
	rapidxml::xml_node<>* g_texture_normal = document.allocate_node(rapidxml::node_element, "NormalTexture", document.allocate_string(terrain.m_GTexturePath[1].c_str()));
	g_texture->append_node(g_texture_diff);
	g_texture->append_node(g_texture_normal);
	terrain_node->append_node(g_texture);

	rapidxml::xml_node<>* b_texure = document.allocate_node(rapidxml::node_element, "BlueTexture");
	rapidxml::xml_node<>* b_texure_diff = document.allocate_node(rapidxml::node_element, "DiffTexture", document.allocate_string(terrain.m_BTexturePath[0].c_str()));
	rapidxml::xml_node<>* b_texure_normal = document.allocate_node(rapidxml::node_element, "NormalTexture", document.allocate_string(terrain.m_BTexturePath[1].c_str()));
	b_texure->append_node(b_texure_diff);
	b_texure->append_node(b_texure_normal);
	terrain_node->append_node(b_texure);	

	node->append_node(terrain_node);
}

void CXmlSceneParser::AddCameraNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, const shared_ptr<CCamera>& camera)
{
	rapidxml::xml_node<>* camera_node = document.allocate_node(rapidxml::node_element, "Camera");
	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	AddVectorToNode(document, position, camera->GetPosition());
	camera_node->append_node(position);

	rapidxml::xml_node<>* pitch_node = document.allocate_node(rapidxml::node_element, "Pitch", document.allocate_string(std::to_string(camera->GetPitch()).c_str()));
	camera_node->append_node(pitch_node);

	rapidxml::xml_node<>* yaw_node = document.allocate_node(rapidxml::node_element, "Yaw", document.allocate_string(std::to_string(camera->GetYaw()).c_str()));
	camera_node->append_node(yaw_node);

	node->append_node(camera_node);
}

void CXmlSceneParser::AddLightNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, const CLight& light)
{
	rapidxml::xml_node<>* light_node = document.allocate_node(rapidxml::node_element, "Light");
	rapidxml::xml_node<>* type_node = document.allocate_node(rapidxml::node_element, "Type", document.allocate_string(std::to_string(light.GetType()).c_str()));
	light_node->append_node(type_node);

	rapidxml::xml_node<>* position = document.allocate_node(rapidxml::node_element, "Position");
	AddVectorToNode(document, position, light.GetPosition());
	light_node->append_node(position);

	rapidxml::xml_node<>* color = document.allocate_node(rapidxml::node_element, "Color");
	AddVectorToNode(document, color, light.GetColour());
	light_node->append_node(color);

	rapidxml::xml_node<>* att = document.allocate_node(rapidxml::node_element, "Attenuation");
	AddVectorToNode(document, att, light.GetAttenuation());
	light_node->append_node(att);

	rapidxml::xml_node<>* cutoff_node = document.allocate_node(rapidxml::node_element, "CutOff", document.allocate_string(std::to_string(light.GetCutoff()).c_str()));
	light_node->append_node(cutoff_node);

	node->append_node(light_node);
}

void CXmlSceneParser::AddSkyBoxNode(rapidxml::xml_document<>& document, rapidxml::xml_node<>* node, const vector<string>& day_textures, const vector<string>& night_textures)
{
	rapidxml::xml_node<>* skybox_node = document.allocate_node(rapidxml::node_element, "SkyBox");
	rapidxml::xml_node<>* day_node = document.allocate_node(rapidxml::node_element, "DayTextures");
	AddCubeTextureToNode(document, day_node, day_textures);
	skybox_node->append_node(day_node);

	rapidxml::xml_node<>* night_node = document.allocate_node(rapidxml::node_element, "NightTextures");
	AddCubeTextureToNode(document, night_node, day_textures);
	skybox_node->append_node(night_node);

	node->append_node(skybox_node);
}
