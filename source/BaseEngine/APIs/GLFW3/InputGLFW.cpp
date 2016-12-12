#include "InputGLFW.h"

CInputGLFW::CInputGLFW(GLFWwindow* window)
{
	m_Window = window;
	ClearKeyBuffer();
}

bool CInputGLFW::GetKeyUp(KeyCodes::Type i)
{
	return false;
}

bool CInputGLFW::GetKey(KeyCodes::Type i)
{
	int glfw_key_scan_code = KeyToGlfw(i);

	if (glfw_key_scan_code >= 0)
	if (m_KeyStates[glfw_key_scan_code])
	{
		return true;
	}
	return false;
}

bool CInputGLFW::GetKeyDown(KeyCodes::Type i)
{
	return false;
}

bool CInputGLFW::GetMouseKeyDown(int key)
{
	return false;
}

bool CInputGLFW::GetMouseKeyUp(int key)
{
	return false;
}

bool CInputGLFW::GetMouseKey(int key)
{
	return false;
}

glm::vec2 CInputGLFW::CalcualteMouseMove()
{
	int w = 320;
	int h = 240;	
	
	glm::vec2 dmove(0);
	dmove.x = w / 2 - m_CurentMousePosition.x;
	dmove.y = h / 2 - m_CurentMousePosition.y;

	if (m_Api != nullptr)
		m_Api->SetCursorPosition(w / 2, h / 2);
	return dmove;
}

glm::vec2 CInputGLFW::GetMousePosition()
{
	return m_CurentMousePosition;
}

void CInputGLFW::SetKeyToBuffer(int key, bool value)
{
	if (key >= 0 && key < m_KeyCount)
		m_KeyStates[key] = value;
}

int CInputGLFW::KeyToGlfw(int key)
{
	if (key == KeyCodes::Q) return GLFW_KEY_Q;
	if (key == KeyCodes::W) return GLFW_KEY_W;
	if (key == KeyCodes::E) return GLFW_KEY_E;
	if (key == KeyCodes::R) return GLFW_KEY_R;
	if (key == KeyCodes::T) return GLFW_KEY_T;
	if (key == KeyCodes::Y) return GLFW_KEY_Y;
	if (key == KeyCodes::U) return GLFW_KEY_U;
	if (key == KeyCodes::I) return GLFW_KEY_I;
	if (key == KeyCodes::O) return GLFW_KEY_O;
	if (key == KeyCodes::P) return GLFW_KEY_P;
	if (key == KeyCodes::A) return GLFW_KEY_A;
	if (key == KeyCodes::S) return GLFW_KEY_S;
	if (key == KeyCodes::D) return GLFW_KEY_D;
	if (key == KeyCodes::F) return GLFW_KEY_F;
	if (key == KeyCodes::G) return GLFW_KEY_G;
	if (key == KeyCodes::H) return GLFW_KEY_H;
	if (key == KeyCodes::J) return GLFW_KEY_J;
	if (key == KeyCodes::K) return GLFW_KEY_K;
	if (key == KeyCodes::L) return GLFW_KEY_L;
	if (key == KeyCodes::Z) return GLFW_KEY_Z;
	if (key == KeyCodes::X) return GLFW_KEY_X;
	if (key == KeyCodes::C) return GLFW_KEY_C;
	if (key == KeyCodes::V) return GLFW_KEY_V;
	if (key == KeyCodes::B) return GLFW_KEY_B;
	if (key == KeyCodes::N) return GLFW_KEY_N;
	if (key == KeyCodes::M) return GLFW_KEY_M;

	if (key == KeyCodes::LCTRL) return GLFW_KEY_LEFT_CONTROL;
	if (key == KeyCodes::ENTER) return GLFW_KEY_ENTER;
	if (key == KeyCodes::SPACE) return GLFW_KEY_SPACE;
	if (key == KeyCodes::ESCAPE) return GLFW_KEY_ESCAPE;

	if (key == KeyCodes::LARROW) return GLFW_KEY_LEFT;
	if (key == KeyCodes::RARROW) return GLFW_KEY_RIGHT;
	if (key == KeyCodes::UARROW) return GLFW_KEY_UP;
	if (key == KeyCodes::DARROW) return GLFW_KEY_DOWN;
	return 0;
}

void CInputGLFW::SetCursorPosition(int x, int y)
{
	m_CurentMousePosition.x = static_cast<float>(x);
	m_CurentMousePosition.y = static_cast<float>(y);
}

void CInputGLFW::ClearKeyBuffer()
{
	for (int x = 0; x < m_KeyCount; x++)
		m_KeyStates[x] = false;
}
