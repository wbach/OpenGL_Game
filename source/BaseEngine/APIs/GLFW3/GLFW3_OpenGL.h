#pragma once
#define GLFW_DLL
#include "../Api.h"
#include <GLFW/glfw3.h>
#include "InputGLFW.h"


class CGlfwOpenGlApi : public CApi
{
public:
	virtual void CreateOpenGLWindow(std:: string window_name, int width, int height, bool full_screen) override;
	virtual int PeekMessages() override;
	virtual void UpdateWindow() override;
	virtual void SetFullScreen(bool full_screen) override;
	virtual bool CheckActiveWindow() override;

	virtual void BeginFrame() override;
	virtual void LockFps(float fps) override;

	virtual void SetInput(std::shared_ptr<CInput>&) override;
	virtual double GetTime() override;
	virtual void ShowCursor(bool show) override;

	virtual void SetCursorPosition(int x, int y) override;
	~CGlfwOpenGlApi();
private:
	static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
	static void CursorPosCallback(GLFWwindow* pWindow, double x, double y);
	static void MouseCallback(GLFWwindow* pWindow, int Button, int Action, int Mode);
	GLFWwindow *m_Window;
	CInput* m_Input;
	double m_StartTime;
};