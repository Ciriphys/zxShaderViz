#include "sppch.h"

#include <Engine.h>

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::shared_ptr<Engine> Engine::s_Instance = nullptr;

Engine::Engine()
{
	m_Window = std::make_shared<Window>();
	m_Window->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));

	m_Minimized = (m_Window->GetWidth() == 0 && m_Window->GetHeight() == 0);
}

void Engine::RenderLoop()
{
	while (m_Window->IsActive())
	{
		m_Window->Update();
		if (m_Minimized) continue;
	
		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//std::cout << "Tick" << std::endl;
	}
}

std::shared_ptr<Engine> Engine::GetEngineInstance()
{
	if (!s_Instance) s_Instance = std::shared_ptr<Engine>(new Engine());
	return s_Instance;
}

void Engine::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Emit<WindowResized>(BIND_FUNCTION(&Engine::OnWindowResized, this));
	dispatcher.Emit<WindowClosed>(BIND_FUNCTION(&Engine::OnWindowClosed, this));
	dispatcher.Emit<KeyPressed>(BIND_FUNCTION(&Engine::OnKeyPressed, this));
}

bool Engine::OnKeyPressed(KeyPressed& e)
{
	m_Window->SetTitle(std::to_string(e.GetKeyCode()));
	return true;
}

bool Engine::OnWindowClosed(WindowClosed& e)
{
	m_Window->Close();
	return true;
}

bool Engine::OnWindowResized(WindowResized& e)
{
	m_Minimized = (e.GetWidth() == 0 && e.GetHeigth() == 0);
	return true;
}
