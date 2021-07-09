#include "sppch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Window.h>

static bool s_glfwInit = false;
static uint32_t s_KeyRepeats = 0;

void Window::SetEventCallbackProcedure(const EventProcedure& eventProcedure)
{
	m_Data.m_Procedure = eventProcedure;
}

void Window::Update()
{
	glfwPollEvents();
	glfwSwapBuffers(m_Window);
}

void Window::Close()
{
	glfwTerminate();
	s_glfwInit = false;
	m_IsActive = false;
}

void Window::SetRefreshRate(RefreshRate rrate)
{
	m_Data.m_RefreshRate = rrate;
	glfwSwapInterval(m_Data.m_RefreshRate);
}

void Window::SetTitle(std::string title)
{
	m_Data.m_Title = title;
	glfwSetWindowTitle(m_Window, m_Data.m_Title.c_str());
}

void Window::Init()
{
	if (!s_glfwInit)
	{
		if (!glfwInit())
		{
			std::cerr << "Couldn't initialize GLFW!" << std::endl;
			// Error Handling
		}

		s_glfwInit = true;
	}

	m_Window = glfwCreateWindow(m_Data.m_Width, m_Data.m_Height, m_Data.m_Title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(m_Window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Couldn't initialize Glad!" << std::endl;
		// Error Handling
	}

	glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(&m_Data));

	// Setup Event Handling
	glfwSetKeyCallback(m_Window, [](GLFWwindow* wnd, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(wnd);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressed e(key, s_KeyRepeats);
				data.m_Procedure(e);
				break;
			}
			case GLFW_RELEASE:
			{
				s_KeyRepeats = 0;
				KeyReleased e(key);
				data.m_Procedure(e);
				break;
			}
			case GLFW_REPEAT:
			{
				s_KeyRepeats++;
				KeyPressed e(key, s_KeyRepeats);
				data.m_Procedure(e);
				break;
			}
			}
		});
	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* wnd, int width, int height)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));
			data.m_Width = width;
			data.m_Height = height;

			WindowResized e(width, height);
			data.m_Procedure(e);
		});
	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* wnd)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			WindowClosed e;
			data.m_Procedure(e);
		});
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* wnd, int button, int action, int mods)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressed e(button);
					data.m_Procedure(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleased e(button);
					data.m_Procedure(e);
					break;
				}
			}
		});
	glfwSetScrollCallback(m_Window, [](GLFWwindow* wnd, double xoffset, double yoffset)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			MouseWheelScrolled e(xoffset, yoffset);
			data.m_Procedure(e);
		});
	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* wnd, double xPos, double yPos)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			MouseMoved e(xPos, yPos);
			data.m_Procedure(e);
		});

	m_IsActive = true;
}
