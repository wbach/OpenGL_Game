#include "GLFW3_OpenGL.h"
#include <thread>
void CGlfwOpenGlApi::CreateOpenGLWindow(std::string window_name, int width, int height, bool full_screen)
{
	if (glfwInit() != 1)
		exit(1);

	int major, minor, rev;

	glfwGetVersion(&major, &minor, &rev);

	printf("GLFW %d.%d.%d initialized\n", major, minor, rev);

	GLFWmonitor* monitor = NULL;

	if (full_screen)
	{
		monitor = glfwGetPrimaryMonitor();
	}

	m_Window = glfwCreateWindow(width, height, window_name.c_str(), monitor, NULL);


	if (!m_Window)
		exit(1);

	glfwMakeContextCurrent(m_Window);	
}

void CGlfwOpenGlApi::UpdateWindow()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void CGlfwOpenGlApi::SetFullScreen(bool full_screen)
{
	GLFWmonitor* monitor = full_screen ? glfwGetPrimaryMonitor() : NULL;

	if (monitor == NULL) return;

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

bool CGlfwOpenGlApi::CheckActiveWindow()
{
	return true;
}

void CGlfwOpenGlApi::ShowCursor(bool show)
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void CGlfwOpenGlApi::SetInput(std::shared_ptr<CInput>& input)
{
	s_input = input = std::make_shared<CInputGLFW>(m_Window);
	input->m_Api = this;
	s_input->m_Api = this;
	glfwSetKeyCallback(m_Window, CGlfwOpenGlApi::KeyCallback);
	glfwSetCursorPosCallback(m_Window, CGlfwOpenGlApi::CursorPosCallback);
	glfwSetMouseButtonCallback(m_Window, CGlfwOpenGlApi::MouseCallback);
}

double CGlfwOpenGlApi::GetTime()
{
	return glfwGetTime();
}

void CGlfwOpenGlApi::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	if (s_input != nullptr) 
	{
		bool key_state = (action == GLFW_RELEASE) ? false : true;
		s_input->SetKeyToBuffer(key, key_state);
	}
	else
	{

	}
		
}

void CGlfwOpenGlApi::CursorPosCallback(GLFWwindow* pWindow, double x, double y)
{
	if (s_input != nullptr)
	{
		s_input->SetCursorPosition(static_cast<int>(x), static_cast<int>(y));
	}
}


void CGlfwOpenGlApi::MouseCallback(GLFWwindow* pWindow, int Button, int Action, int Mode)
{

}

void CGlfwOpenGlApi::BeginFrame()
{
	m_StartTime = glfwGetTime();
}

void CGlfwOpenGlApi::LockFps(float fps)
{
	//double t = static_cast<double>(1000.0f / fps) - (glfwGetTime() - m_StartTime);
	//std::cout << t << std::endl;
	//if (static_cast<double>(1000.0f / fps) >  glfwGetTime() - m_StartTime)  std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<int>( t * 1000000 )));
}

void CGlfwOpenGlApi::SetCursorPosition(int x, int y)
{
	glfwSetCursorPos(m_Window, x, y);
}

CGlfwOpenGlApi::~CGlfwOpenGlApi()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

int CGlfwOpenGlApi::PeekMessages()
{
	BeginFrame();
	return glfwWindowShouldClose(m_Window);
}
