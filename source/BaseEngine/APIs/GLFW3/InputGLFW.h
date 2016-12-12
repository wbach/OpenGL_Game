#pragma once
#include "../../Input/Input.h"
#include <GLFW/glfw3.h>

class CInputGLFW : public CInput
{
public:
	CInputGLFW(GLFWwindow* window);
	// Keyboard
	virtual bool GetKeyUp(KeyCodes::Type i) override;
	virtual bool GetKey(KeyCodes::Type i) override;
	virtual bool GetKeyDown(KeyCodes::Type i) override;

	//Mouse
	virtual bool GetMouseKeyDown(int key) override;
	virtual bool GetMouseKeyUp(int key) override;
	virtual bool GetMouseKey(int key) override;
	virtual glm::vec2 CalcualteMouseMove() override;
	virtual glm::vec2 GetMousePosition() override;

	//Callbacks
	virtual void ClearKeyBuffer() override;
	virtual void SetKeyToBuffer(int key, bool value) override;
	virtual void SetCursorPosition(int x, int y) override;
private:
	int KeyToGlfw(int key);

	static const int m_KeyCount = 350;
	bool m_KeyStates[m_KeyCount];
	GLFWwindow* m_Window;
	glm::vec2 m_CurentMousePosition = glm::vec2(0);
};