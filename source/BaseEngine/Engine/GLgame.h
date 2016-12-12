#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include "../Utils/Utils.h"
#include "Scene.h"
#include "../Display/DisplayManager.h"
#include "../Shaders/LoadingShader.h"
#include "../GUI/GUIRenderer.h"
#include "../Input/InputManager.h"
#include "../Renderers/MasterRenderer.h"
#include "../Loader/XML/XMLSceneParser.h"
#include "../Renderers/Simple/SimpleMasterRenderer.h"

#include <mutex>
using namespace std;
struct SSimpleQuad
{
	bool isInit = false;
	int m_IndicesSize = 0;
	GLuint vao, i_vbo, vbo_id, vbo_text_id;
	void Init() {
		Utils::CreateQuad(vao, i_vbo, vbo_id, vbo_text_id, m_IndicesSize);
		isInit = true;
	}
	~SSimpleQuad()
	{
		if(isInit)
		Utils::DeleteQuad(vao, i_vbo, vbo_id, vbo_text_id);
	}
};
struct SLoadingScreenData
{
	SSimpleQuad quad;
	GLuint		circleTexture;
	GLuint		backgroundTexture;
	glm::mat4	transformation_matrix;
	void Init(CLoader& loader)
	{	
		quad.Init();

		int texture = loader.LoadTextureImmediately("Data/GUI/circle2.png", false, true);
		circleTexture = loader.m_Textures[texture]->GetId();

		texture = loader.LoadTextureImmediately("Data/GUI/black-knight-dark-souls.png", true, true);
		backgroundTexture = loader.m_Textures[texture]->GetId();

		transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0.81, -0.75, -0.01), glm::vec3(0), glm::vec3(0.1));
	}

};
class CGame 
{
public:
	CGame();
	~CGame();
	bool m_FroceQuit = false;

	virtual void Initialize(std::shared_ptr<CApi>& api);
	void GameLoop();
	virtual void MainGameLoop();
	void PhysicsLoop();

	void SetWindowSize(glm::vec2 size);

	CGUIRenderer& GetGUIRenderer() { return m_GuiRenderer; }
	CInputManager& GetInputManager() { return m_InputManager; }
	CDisplayManager& GetDisplayManager() { return m_DisplayManager; }
	CMasterRenderer& GetMasterRenderer() { return m_MasterRenderer; }

	void CreateProjectionMatrix();
	const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

    int SetCurrentScene(int i);
    void LoadScene();
	void RenderLoadingScreen() ;
	void ClearScenes();
	void AddScene(shared_ptr<CScene>& scene);

	void RenderStartSeries();
	float Fade(float delta_time);
	float FadeIn(float delta_time, float start_time, float durration);
	float FadeOut(float delta_time, float start_time, float durration);

	virtual int ReadConfiguration(string file_name);

	void(*OnGameLoopRun)();

	//For editor
	CScene* GetCurrentScene();	

	CXmlSceneParser m_SceneParser;
	
	const float& GetShadowMapSize();
	const bool& IsShadows();

	//Loading in this thread
	bool m_IsLoadedFromFile = false;
	std::string m_SceneFileToLoad;
	
	const glm::vec2& GetMaxTextureResolution();

	void SetIsLoading(bool is);
	bool GetIsLoading();

protected:
	std::mutex m_Mutex;

	CDisplayManager m_DisplayManager;
	CInputManager	m_InputManager;

	LoadingShader m_LoadingShader;
	SLoadingScreenData m_LoadingScreenData;

	vector<shared_ptr<CScene>> m_Scenes;
	CScene* m_CurrScene;

	CGUIRenderer m_GuiRenderer;
	CMasterRenderer m_MasterRenderer;
	CSimpleMasterRenderer m_SimpleRenderer;

	glm::vec3 m_BackgroundColour;
	glm::mat4 m_ProjectionMatrix;

	float m_ViewDistance;
	float m_GrassViewDistance;

	bool	m_IsShadows;
	float	m_ShadowsDistance;
	float	m_ShadowMapSize;

	float m_RenderingResolutionModifier = 1;
	float m_WaterQuality;
	glm::vec2	m_ReflectionSize;
	glm::vec2	m_RefractionSize;
	glm::vec2 m_MaxTextureResolution;

	bool m_IsLoading;
	bool m_IsFullScreen;	

	glm::vec2	m_WindowSize;
	string		m_WindowName;
	int		m_RefreshRate;

	const float m_NearPlane = 0.1f;
	const float m_FarPlane = 1000;
	const float m_Fov = 50;

	bool	m_IsSound;
	float	m_SoundVolume;

	int m_ApiMessage;


	std::mutex m_PhysicsSyncMutex;
	//bool m_MainLoopStatus = false;
	//bool m_PhysicLoopStatus = false;

	bool m_UsingSimpleRender = false;
};



