#pragma once
#include "../Input/Input.h"
#include <memory>

class CInput;
static std::shared_ptr<CInput> s_input = nullptr;
namespace ApiMessages 
{
	enum 
	{
		NONE = 0,
		QUIT
	};
}

class CApi 
{
public:
	virtual void CreateOpenGLWindow(std::string window_name, int width, int height, bool full_screen) = 0;
	virtual int PeekMessages() = 0;
	virtual void UpdateWindow() = 0;
	virtual void SetFullScreen(bool full_screen) = 0;
	virtual bool CheckActiveWindow() = 0;
	virtual void ShowCursor(bool show) = 0;
	virtual void SetInput(std::shared_ptr<CInput>&) = 0;
	virtual double GetTime() = 0;	
	virtual void SetCursorPosition(int x, int y) = 0;

	//Take time function to lock fps, shuld be on begin main loop
	virtual void BeginFrame() = 0;
	virtual void LockFps(float fps) = 0;
};