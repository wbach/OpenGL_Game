#include "GLgame.h"
#include "../Input/InputManager.h"
CGame::CGame()
: m_DisplayManager()
, m_InputManager()
, m_MasterRenderer()
, m_GuiRenderer()
, m_LoadingShader()
, m_BackgroundColour(0.6, 0.6, 0.8)
, m_WindowSize(1000, 600)
, m_WindowName("GL game")
, m_IsSound(true)
, m_IsShadows(true)
, m_GrassViewDistance(500)
, m_IsFullScreen(false)
, m_RefreshRate(60)
, m_ShadowMapSize(1024)
, m_SoundVolume(1.0)
, m_WaterQuality(1)
, m_ViewDistance(1000)
, m_MaxTextureResolution(4096)

{
	ReadConfiguration("./Data/Conf.ini");
}

CGame::~CGame()
{
	std::cout << "Destructor " << typeid(*this).name() << endl;
}

void CGame::Initialize(std::shared_ptr<CApi>& api)
{
	m_DisplayManager.SetApi(api);
	m_DisplayManager.Initialize(m_WindowName, Renderer::OPENGL, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y), m_IsFullScreen);

	CreateProjectionMatrix();

	m_GuiRenderer.Init(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));


	m_DisplayManager.SetInput(m_InputManager.m_Input);
//	m_DisplayManager.SetFullScreen(m_IsFullScreen);
	m_DisplayManager.SetRefreshRate(m_RefreshRate);

	//renderStartSeries();

	m_LoadingScreenData.Init(m_CurrScene->GetLoader());
	m_LoadingShader.Init();
	m_LoadingShader.Start();
	m_LoadingShader.LoadAlphaValue(1.f);

	m_IsLoading = true;
	std::thread l(&CGame::LoadScene, this);

	int x = 0;
	bool  load = true;
	m_DisplayManager.GetSync() = true;
	bool post_load = false;
	while (load)
	{
		m_DisplayManager.PeekMessage();

		load = GetIsLoading();

		COpenGLObject* obj = m_CurrScene->GetLoader().GetObjectToOpenGLLoadingPass();
		if (obj != nullptr)
		{
			load = true;
			obj->OpenGLLoadingPass();
		}

		RenderLoadingScreen();
	}
	l.join();
	load = true;
	while (load)
	{
		m_DisplayManager.PeekMessage();
		COpenGLObject* obj = m_CurrScene->GetLoader().GetObjectToOpenGLPostLoadingPass();
		if (obj != nullptr)
		{
			load = true;
			obj->OpenGLPostLoadingPass();
		}
		else
		{
			load = false;
		}
		RenderLoadingScreen();
	}
	m_LoadingShader.Stop();


	m_MasterRenderer.Init(m_CurrScene->GetCamera().get(), m_WindowSize, m_ProjectionMatrix,
		m_Fov, m_NearPlane, m_FarPlane, m_RenderingResolutionModifier, m_ShadowMapSize, m_ShadowsDistance,
		m_WaterQuality, m_ReflectionSize, m_RefractionSize, m_ViewDistance
	);
	m_SimpleRenderer.Init(m_CurrScene->GetCamera().get(), m_WindowSize, m_ProjectionMatrix);
	m_SimpleRenderer.SetSkyBoxMesh(m_MasterRenderer.GetSkyBoxRenderer().GetMesh());
	m_SimpleRenderer.SetSkyBoxTextures(m_MasterRenderer.GetSkyBoxRenderer().GetDayTexture(), m_MasterRenderer.GetSkyBoxRenderer().GetNightTexture());
}
void CGame::SetWindowSize(glm::vec2 size)
{
	m_WindowSize = size;
}
void CGame::GameLoop()
{	
	m_ApiMessage = ApiMessages::NONE;

 //   string fps_text = "FPS : ";
	//string object_count_text = "Objects : ";
	//string vertex_count_text = "Vertex : ";
	//string game_time = "Game Time : ";
	//vector<CGUIText> texts;

	//glm::vec2 shadow_offset(-0.002);
	//glm::vec2 fps_pos(-0.9475, 0.8575);
	//unsigned int sfps_id = m_GuiRenderer.AddText("sfps",fps_text, fps_pos + shadow_offset, 1, glm::vec3(0, 0, 0));
	//unsigned int fps_id = m_GuiRenderer.AddText("fps", fps_text, fps_pos, 1, glm::vec3(0.9));
	//m_GuiRenderer.GetElement(sfps_id)->m_SaveToFile = false;
	//m_GuiRenderer.GetElement(fps_id)->m_SaveToFile = false;
	//
	//glm::vec2 oc(-0.9475, 0.7575);
	//unsigned int soc_id = m_GuiRenderer.AddText("soc", object_count_text, oc + shadow_offset, 1, glm::vec3(0, 0, 0));
	//unsigned int oc_id = m_GuiRenderer.AddText("oc",object_count_text, oc, 1, glm::vec3(0.9));
	//m_GuiRenderer.GetElement(soc_id)->m_SaveToFile = false;
	//m_GuiRenderer.GetElement(oc_id)->m_SaveToFile = false;

	//glm::vec2 vc(-0.9475, 0.6575);
	//unsigned int svc_id = m_GuiRenderer.AddText("svc", vertex_count_text, vc + shadow_offset, 1, glm::vec3(0, 0, 0));
	//unsigned int vc_id = m_GuiRenderer.AddText("vc", vertex_count_text, vc, 1, glm::vec3(0.9));
	//m_GuiRenderer.GetElement(svc_id)->m_SaveToFile = false;
	//m_GuiRenderer.GetElement(vc_id)->m_SaveToFile = false;

	//glm::vec2 tc(0.6475, 0.9075);
	//unsigned int stc_id = m_GuiRenderer.AddText("stc", game_time, tc + shadow_offset, .5f, glm::vec3(0, 0, 0));
	//unsigned int tc_id = m_GuiRenderer.AddText("tc", game_time, tc, .5f, glm::vec3(0.9));
	//m_GuiRenderer.GetElement(stc_id)->m_SaveToFile = false;
	//m_GuiRenderer.GetElement(tc_id)->m_SaveToFile = false;

	//texts.push_back(CGUIText("Lorem Ipsum is simply dummy text of the printing and typesetting industry.\n Lorem Ipsum has been the industry's standard dummy text ever since the 1500s.", glm::vec2(-0.90, 0.7), 1, glm::vec3(1, 1, 1)));
	
	
	vector<CGUITexture> debug_textures;
	//debug_textures.push_back(CGUITexture(m_MasterRenderer.GetShadowMap(), glm::vec2(0.5), glm::vec2(0.25,0.25) ));
	

	std::thread physics_thread(&CGame::PhysicsLoop, this);

	while (m_ApiMessage != ApiMessages::QUIT && !m_FroceQuit)
	{	
		
		m_ApiMessage = m_DisplayManager.PeekMessage();

		if (m_InputManager.GetKey(KeyCodes::ESCAPE))
			m_ApiMessage = ApiMessages::QUIT;

		if (OnGameLoopRun != nullptr)
			OnGameLoopRun();

		MainGameLoop();
	
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_BackgroundColour.x, m_BackgroundColour.y, m_BackgroundColour.z, 1);
	
		if (m_CurrScene != nullptr)
		{
			//GetCurrentScene()->m_PhysicsScene.Update(static_cast<float>(m_DisplayManager.GetDeltaTime()));
		//	m_CurrScene->ApplyPhysicsToObjects(static_cast<float>(m_DisplayManager.GetDeltaTime()));
			switch (m_CurrScene->Update())
			{
			case 1: m_ApiMessage = ApiMessages::QUIT; break;
			}
			//m_MasterRenderer.Render(m_CurrScene, m_IsShadows);
			m_SimpleRenderer.Render(m_CurrScene);

			//m_MasterRenderer.DebugRenderTextures();

			if(m_CurrScene->GetGUI() != nullptr)
				m_GuiRenderer.Render(*m_CurrScene->GetGUI());
		}
		m_DisplayManager.Update();
		m_InputManager.CheckReleasedKeys();

		int objects_per_frame = m_MasterRenderer.GetObjectsPerFrame();
		int vertex_per_frame = m_MasterRenderer.GetVertexPerFrame();

		/*m_GuiRenderer.UpdateText(sfps_id, fps_text + std::to_string(m_DisplayManager.GetFps()));
		m_GuiRenderer.UpdateText(fps_id, fps_text + std::to_string(m_DisplayManager.GetFps()));

		m_GuiRenderer.UpdateText(soc_id, object_count_text + std::to_string(objects_per_frame));
		m_GuiRenderer.UpdateText(oc_id, object_count_text + std::to_string(objects_per_frame));

		m_GuiRenderer.UpdateText(svc_id, vertex_count_text + std::to_string(vertex_per_frame));
		m_GuiRenderer.UpdateText(vc_id, vertex_count_text + std::to_string(vertex_per_frame));

		int hour, minutes;
		m_CurrScene->m_DayNightCycle.GetCurrentHour(hour, minutes);

		m_GuiRenderer.UpdateText(stc_id, game_time + std::to_string(hour) + " : " + std::to_string(minutes));
		m_GuiRenderer.UpdateText(tc_id, game_time + std::to_string(hour) + " : " + std::to_string(minutes));*/

	}
	physics_thread.join();
}
void CGame::MainGameLoop()
{
	
}
void CGame::PhysicsLoop()
{
	int m_FrameCount = 0;
	float m_Fps = 0;
	float m_CurrentTime = 0, m_PreviousTime = 0;
	float time_interval =0;
	while (m_ApiMessage != ApiMessages::QUIT && !m_FroceQuit)
	{
		if (m_CurrScene == nullptr) continue;

		auto start = std::chrono::high_resolution_clock::now();
	
		m_PreviousTime = m_CurrentTime;
		m_CurrentTime = m_DisplayManager.GetCurrentTime();

		float dt = m_CurrentTime - m_PreviousTime;

		time_interval += dt;

		if (time_interval > 1)
		{
			m_Fps = m_FrameCount /time_interval;
			m_PreviousTime = m_CurrentTime;
			m_FrameCount = 0;
			time_interval = 0;
		//	std::cout << m_Fps << std::endl;

		}
		m_FrameCount++;
		m_CurrScene->ApplyPhysicsToObjects(static_cast<float>(dt));

		auto end = std::chrono::high_resolution_clock::now();

		if (std::chrono::milliseconds(16) >  std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
			std::this_thread::sleep_for(std::chrono::milliseconds(16) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
		std::lock_guard<std::mutex>::lock_guard(m_PhysicsSyncMutex);
	}

}
void CGame::LoadScene()
{
	if (m_CurrScene == nullptr) return;

	m_CurrScene->Initialize();
	m_SceneParser.LoadScene(GetCurrentScene()->m_SceneFile, GetCurrentScene());
	SetIsLoading(false);

}
float CGame::Fade(float delta_time)
{
	bool change = false;
	float alpha = 0.0;
	if (delta_time < 500) change = true;
	if (delta_time < 2000 && delta_time > 500) 
	{

		alpha = (static_cast<float>(delta_time) - 500.0f) / 2000.0f;
		if (alpha > 1)
			alpha = 1.0;
		change = true;
	}
	if (delta_time > 4000 )
	{
		alpha = ((6000.0f - static_cast<float>(delta_time)) / 2000.0f);
		if (alpha < 0)
			alpha = 0;
		change = true;
	}
	if (!change)
		return 1.0;

	return alpha;
}
float CGame::FadeIn(float delta_time, float start_time, float durration)
{
	float alpha = 0.0;
	alpha = (static_cast<float>(delta_time) - static_cast<float>(start_time)) / static_cast<float>(durration);
	if (alpha > 1)
		alpha = 1.0;
	return alpha;
}
float CGame::FadeOut(float delta_time, float start_time, float durration)
{
	float alpha = 0.0;
	alpha = ((static_cast<float>(start_time + durration) - static_cast<float>(delta_time)) / static_cast<float>(durration));
	if (alpha < 0)
		alpha = 0;

	return alpha;
}
int CGame::ReadConfiguration(string file_name)
{
	ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open configuration file." << std::endl;
		return -1;
	}
	string line;

	while (std::getline(file, line))
	{
		string var = line.substr(0, line.find_last_of("="));
		string value = line.substr(line.find_last_of("=") + 1);

		if (var.compare("Name") == 0)				m_WindowName	= value; 
		if (var.compare("Resolution") == 0)			m_WindowSize	= Get::Vector2d(value);
		if (var.compare("FullScreen") == 0)			m_IsFullScreen	= Get::Boolean(value);
		if (var.compare("RefreshRate") == 0)		m_RefreshRate	= Get::Int(value);
		if (var.compare("Sound") == 0)				m_IsSound		= Get::Boolean(value);
		if (var.compare("SoundVolume") == 0)		m_SoundVolume	= Get::Float(value);
		if (var.compare("RenderingResolution") == 0)	m_RenderingResolutionModifier = Get::Float(value);
		if (var.compare("WaterQuality") == 0)		m_WaterQuality	= Get::Float(value);
		if(var.compare("WaterReflectionResolution") == 0)	m_ReflectionSize = Get::Vector2d(value);
		if(var.compare("WaterRefractionResolution") == 0)	m_RefractionSize = Get::Vector2d(value);
		if (var.compare("TextureMaxResolution") == 0)	m_MaxTextureResolution = Get::Vector2d(value);
		if (var.compare("Shadows") == 0)			m_IsShadows		= Get::Boolean(value);
		if (var.compare("ShadowsDistance") == 0)	m_ShadowsDistance = Get::Float(value);
		if (var.compare("ShadowMapSize") == 0)		m_ShadowMapSize	= Get::Float(value);
		if (var.compare("ViewDistance") == 0)		m_ViewDistance  = Get::Float(value);
		if (var.compare("GrassViewDistance") == 0)	m_GrassViewDistance = Get::Float(value);
		if (var.compare("SimpleRender") == 0)		m_UsingSimpleRender = Get::Boolean(value);
		
	}	
	file.close();

	return 0;
	//FILE *f = fopen("Data/Conf.ini", "r");
	//if (f == NULL)
	//{
	//	return -1;
	//}
	//int k;
	//fscanf(f, "Resolution=%ix%i\n", &m_WindowSize.x, &m_WindowSize.y);
	//fscanf(f, "FullScreen=%i\n", &k); if (k == 1) m_IsFullScreen = true; else m_IsFullScreen = false;
	//fscanf(f, "Sound=%i\n", &k); if (k == 1) m_IsSound = true; else m_IsSound = false;
	//fscanf(f, "SoundVolume=%f\n", &m_SoundVolume);
	//fscanf(f, "WaterQuality=%i\n", &m_WaterQuality); 
	//fscanf(f, "Shadows=%i\n", &k); if (k == 1) m_IsShadows = true; else m_IsShadows = false;
	//fscanf(f, "ShadowsResolution=%f\n", &m_ShadowMapSize);
	//fscanf(f, "ViewDistance=%f\n", &m_ViewDistance);
	//fscanf(f, "GrassViewDistance=%f\n", &m_GrassViewDistance);
	//fclose(f);
	return 0;
}
CScene* CGame::GetCurrentScene()
{
	return m_CurrScene;
}

const float & CGame::GetShadowMapSize()
{
	return m_ShadowMapSize;
}

const bool & CGame::IsShadows()
{
	return m_IsShadows;
}

const glm::vec2 & CGame::GetMaxTextureResolution()
{
	return m_MaxTextureResolution;
}

void CGame::SetIsLoading(bool is)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_IsLoading = is;
}

bool CGame::GetIsLoading()
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	return m_IsLoading;
}


void CGame::RenderStartSeries()
{
	//vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };
	//vector<float> text_coords = {
	//	0, 0,
	//	0, 1,
	//	1, 1,
	//	1, 0 };
	//vector<unsigned int>indices = { 0, 1, 3, 3, 1, 2 };

	//GLuint vao		   = Utils::CreateVao();
	//GLuint i_vbo	   = Utils::BindIndicesBuffer(indices);
	//GLuint vbo_id	   = Utils::StoreDataInAttributesList(0, 3, vertex);
	//GLuint vbo_text_id = Utils::StoreDataInAttributesList(1, 2, text_coords);
	//Utils::UnbindVao();
	//m_LoadingShader.Init();

	//GLuint texture = m_CurrScene->GetLoader().LoadTexture("Data/GUI/start1.png",true);
	//glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(2.0));

	//Uint32 start,start2 = SDL_GetTicks();

	//Uint32 delta_time;
	//while (1) 
	//{
	//	delta_time = SDL_GetTicks() - start2;
	//	if (delta_time > 6000) break;
	//	start = SDL_GetTicks();

	//	glEnable(GL_DEPTH_TEST);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(0, 0, 0, 1);

	//	m_LoadingShader.Start();

	//	if (delta_time > 500 && delta_time < 2500) {
	//		m_LoadingShader.LoadAlphaValue(FadeIn(delta_time, 500, 2000));
	//	}
	//	if (delta_time > 4000) {
	//		m_LoadingShader.LoadAlphaValue(FadeOut(delta_time, 4000, 2000));
	//	}

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	m_LoadingShader.LoadTransformMatrix(transformation_matrix);
	//	glBindVertexArray(vao);
	//	glEnableVertexAttribArray(0);
	//	glEnableVertexAttribArray(1);
	//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	glDisableVertexAttribArray(1);
	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);
	//	m_LoadingShader.Stop();
	//	m_DisplayManager.Update();

	//	if ( static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	//}
	//m_LoadingShader.Stop();
	//glDeleteTextures(1, &texture);
	//glDeleteBuffers(1, &i_vbo);
	//glDeleteBuffers(1, &vbo_id);
	//glDeleteBuffers(1, &vbo_text_id);
	//glDeleteVertexArrays(1, &vao);
}
void CGame::RenderLoadingScreen()
{
	// render
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.117, 0.117, 0.117, 1);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_LoadingScreenData.circleTexture);
	m_LoadingScreenData.transformation_matrix *= glm::rotate(-1.f, 0.0f, 0.0f, 1.0f);
	m_LoadingShader.LoadTransformMatrix(m_LoadingScreenData.transformation_matrix);
	Utils::SimpleRenderVao(m_LoadingScreenData.quad.vao, m_LoadingScreenData.quad.m_IndicesSize, 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_LoadingScreenData.backgroundTexture);
	m_LoadingShader.LoadTransformMatrix(glm::mat4(1.f));
	Utils::SimpleRenderVao(m_LoadingScreenData.quad.vao, m_LoadingScreenData.quad.m_IndicesSize, 2);	
	
	
	m_DisplayManager.Update();
}
void CGame::ClearScenes()
{
	m_Scenes.clear();
}
void CGame::CreateProjectionMatrix()
{
	float aspect_ratio = (float)m_WindowSize.x / (float)m_WindowSize.y;
	float y_scale = (float)((1.0f / tan(Utils::ToRadians(m_Fov / 2.0f))));
	float x_scale = y_scale / aspect_ratio;
	float frustum_length = m_FarPlane - m_NearPlane;

	m_ProjectionMatrix[0][0] = x_scale;
	m_ProjectionMatrix[1][1] = y_scale;
	m_ProjectionMatrix[2][2] = -((m_FarPlane + m_NearPlane) / frustum_length);
	m_ProjectionMatrix[2][3] = -1;
	m_ProjectionMatrix[3][2] = -((2 * m_NearPlane * m_FarPlane) / frustum_length);
	m_ProjectionMatrix[3][3] = 0;
}
void CGame::AddScene(shared_ptr<CScene>& scene)
{
	m_Scenes.push_back(scene);
}
int CGame::SetCurrentScene(int i)
{
	for (unsigned int x = 0 ; x < m_Scenes.size() ; x++ )
	{
		if (x == i)
		{
			m_CurrScene = m_Scenes[x].get();
			return 1;
		}
	}
	return -1;
}
