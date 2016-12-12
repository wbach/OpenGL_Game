#include "Editor/SceneEditor.h"
#include "BaseEngine/Engine/GLgame.h"
#include "BaseEngine/APIs/GLFW3/GLFW3_OpenGL.h"
#include "BaseEngine/APIs/SDL2/SDLOpenGL.h"
//#include "TestGame/Test_Scene.h"
//#include "TestGame/SimpleScene.h"
#include "Editor\Scene\EngineEditorScene.h"
#include "RPGpack\Scenes\MainScene.h"
#include <map>

#include <stdlib.h>
#include <crtdbg.h>

std::shared_ptr<CApi> api;

CGame myGame;
#ifdef EDITOR
CSceneEditor editor(myGame);
#endif


int main(int argc, char *argv[])
{	
	//Test git cooment
	//char Title[512]; ZeroMemory(Title, 512); GetConsoleTitle(Title, 512);	HWND console = FindWindow(NULL, Title); ShowWindow(console, NULL);
	//  shared_ptr<CScene> mainMenu = make_shared<CTestMainMenu>(myGame);

	int test_scene_camera_type = 0;
#ifdef EDITOR
	myGame.SetWindowSize(glm::vec2(820, 480));
	api = std::make_shared<CSdlOpenGlApi>();
	test_scene_camera_type = 1;
#else
	//api = std::make_shared<CGlfwOpenGlApi>();
	api = std::make_shared<CSdlOpenGlApi>();
#endif// test_scene_camera_type
	std::shared_ptr<CRPGMainScene> scene = std::make_shared<CRPGMainScene>(&myGame);
	//std::shared_ptr<CSimpleScene> scene = std::make_shared<CSimpleScene>(&myGame);
	std::shared_ptr<CScene> s = scene;
	myGame.AddScene(s);
	if (myGame.SetCurrentScene(0) < 0)
	{
		cout << "[Error] Scene not found." << endl;
	}
	myGame.Initialize(api);
#ifdef EDITOR	
	//std::thread  a(&CSceneEditor::Init, editor, 1366, 768);
	editor.Init(1366, 768);
	editor.PeekMesages();
	editor.m_EditorScene = scene.get();
	myGame.OnGameLoopRun = [] { editor.PeekMesages(); };
	//myGame.m_SceneParser.LoadScene(myGame.GetCurrentScene()->m_SceneFile, myGame.GetCurrentScene());
#else
	//myGame.m_SceneParser.LoadScene(myGame.GetCurrentScene()->m_SceneFile, myGame.GetCurrentScene());
	myGame.GetCurrentScene()->PostInitialize();
#endif
	myGame.GameLoop();

	return 0;
}
