#pragma once
#include "../Api.h"
#include <SDL2/SDL.h>
#include "InputSDL.h"

class CSdlOpenGlApi : public CApi
{
public:
	~CSdlOpenGlApi();
	virtual void CreateOpenGLWindow(std::string window_name, int width, int height, bool full_screen) override;
	virtual int PeekMessages() override;
	virtual void UpdateWindow() override;
	virtual void SetFullScreen(bool full_screen) override;
	virtual bool CheckActiveWindow() override;

	virtual void BeginFrame() override;
	virtual void LockFps(float fps) override;

	virtual void ShowCursor(bool show) override;
	virtual void SetInput(std::shared_ptr<CInput>&) override;
	virtual double GetTime() override;
	virtual void SetCursorPosition(int x, int y) override;
private:
	SDL_GLContext m_GlContext;
	SDL_Window* m_Window;	
	SDL_Event m_Event;	
	Uint32 m_StartTime;
};