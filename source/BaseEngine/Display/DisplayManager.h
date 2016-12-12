#pragma once
#include "../Engine/OpenGLRenderer.h"
#include <iostream>
#include <thread>
#include "glm/glm.hpp"
#include "../Input/Input.h"
using namespace std;
namespace Renderer
{
	enum
	{
		OPENGL = 0,
		DIRECTX
	};
}
class CDisplayManager 
{
public:
	int Initialize(string window_name, int renderer, int w, int h, bool full_screen);
	int PeekMessage();
	void Update();

	void SetRefreshRate(int rate);
	void SetFullScreen(bool full_screen);
	void CalculateFPS();
	const int GetFps();
	const int& GetFPSCap() { return m_FPS_CAP; }

	const float GetCurrentTime();
	const double GetDeltaTime() const { return m_Delta; }

	const glm::vec2& GetWindowSize();

	void ShowCoursor(bool show);
	bool CheckActiveWindow();

	void SetApi(std::shared_ptr<CApi>& api);
	void SetInput(std::shared_ptr<CInput>&);

	void EnableTime() { m_Time = true; }
	void DisableTime() { m_Time = false; }

	bool& GetSync() { return m_Sync; }
private:	
	std::shared_ptr<CApi> m_Api;
	std::shared_ptr<CRenderer> m_Renderer;

	bool m_Time = true;

	double m_LastFrameTime;
	double m_Delta;
	float m_CurrentTime, m_PreviousTime;
	float m_FrameCount, m_Fps;

	int m_FPS_CAP;
	bool m_IsFullScreen;
	glm::vec2 m_WindowsSize;

	bool m_Sync = true;
};