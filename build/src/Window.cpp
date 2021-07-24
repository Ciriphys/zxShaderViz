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
	mData.mProcedure = eventProcedure;
}

void Window::Update()
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void Window::Clear()
{
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Close()
{
	glfwTerminate();
	s_glfwInit = false;
	mData.mIsActive = false;
}

uint32_t Window::GetCurrPosX()
{
	glfwGetWindowPos(mWindow, &mData.mSettings.windowPosx, nullptr);
	return uint32_t(mData.mSettings.windowPosx);
}

uint32_t Window::GetCurrPosY()
{
	glfwGetWindowPos(mWindow, nullptr, &mData.mSettings.windowPosy);
	return uint32_t(mData.mSettings.windowPosy);
}

void Window::SetRefreshRate(RefreshRate rrate)
{
	mData.mSettings.refreshRate = rrate;
	glfwSwapInterval(mData.mSettings.refreshRate);
}

void Window::SetTitle(std::string title)
{
	mData.mTitle = title;
	glfwSetWindowTitle(mWindow, mData.mTitle.c_str());
}

void Window::Init(WindowSettings settings)
{
	if (!s_glfwInit)
	{
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			std::cerr << "Couldn't initialize GLFW!" << std::endl;
			// Error Handling
		}

		s_glfwInit = true;
	}

	mWindow = glfwCreateWindow(mData.mSettings.windowSizex, mData.mSettings.windowSizey, mData.mTitle.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Couldn't initialize Glad!" << std::endl;
		// Error Handling
	}

	glfwSetWindowPos(mWindow, settings.windowPosx, settings.windowPosy);
	glfwSetWindowUserPointer(mWindow, reinterpret_cast<void*>(&mData));
	glViewport(0, 0, mData.mSettings.windowSizex, mData.mSettings.windowSizey);

	// Setup Event Handling
	glfwSetKeyCallback(mWindow, [](GLFWwindow* wnd, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(wnd);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressed e(key, s_KeyRepeats);
					data.mProcedure(e);
					break;
				}
				case GLFW_RELEASE:
				{
					s_KeyRepeats = 0;
					KeyReleased e(key);
					data.mProcedure(e);
					break;
				}
				case GLFW_REPEAT:
				{
					s_KeyRepeats++;
					KeyPressed e(key, s_KeyRepeats);
					data.mProcedure(e);
					break;
				}
			}
		});

	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* wnd, int width, int height)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));
			data.mSettings.windowSizex = width;
			data.mSettings.windowSizey = height;

			WindowResized e(width, height);
			data.mProcedure(e);
		});

	glfwSetWindowPosCallback(mWindow, [](GLFWwindow* wnd, int posx, int posy) 
	{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));
			data.mSettings.windowPosx = posx;
			data.mSettings.windowPosy = posy;

			WindowMoved e(posx, posy);
			data.mProcedure(e);
	});
	

	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* wnd)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			WindowClosed e;
			data.mProcedure(e);
		});

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* wnd, int button, int action, int mods)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressed e(button);
					data.mProcedure(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleased e(button);
					data.mProcedure(e);
					break;
				}
			}
		});

	glfwSetScrollCallback(mWindow, [](GLFWwindow* wnd, double xoffset, double yoffset)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			MouseWheelScrolled e(xoffset, yoffset);
			data.mProcedure(e);
		});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* wnd, double xPos, double yPos)
		{
			WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

			MouseMoved e(xPos, yPos);
			data.mProcedure(e);
		});

	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* wnd, int width, int height) {
			glViewport(0, 0, width, height);
	});

	glfwSetDropCallback(mWindow, [](GLFWwindow* wnd, int num, const char* files[]) {
		WindowData& data = *reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(wnd));

		FilesDropped e(num, files);
		data.mProcedure(e);
	});

	mData.mIsActive = true;
}
