#include "sppch.h"

#include <Engine.h>
#include <Utils/TimeStep.h>

std::shared_ptr<Engine> Engine::s_Instance = nullptr;

Engine::Engine()
{
	m_Window = std::make_shared<Window>();
	m_Window->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));

	m_Renderer = Renderer::GetRenderer();

	m_Minimized = (m_Window->GetWidth() == 0 && m_Window->GetHeight() == 0);
}

void Engine::RenderLoop()
{
	TimeStep ts;

	while (m_Window->IsActive())
	{
		m_Window->Update();
		if (m_Minimized) continue;

		ts.Update();

		m_Window->Clear();
		m_Renderer->GetShader()->SetUniform1f("u_Time", ts.GetExecutionTimef());
		m_Renderer->GetShader()->SetUniformVec2("u_MousePos", mousePos);
		m_Renderer->GetShader()->SetUniformVec2("u_Resolution", { m_Window->GetWidth(), m_Window->GetHeight() });
		m_Renderer->Draw();
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
	dispatcher.Emit<MouseMoved>(BIND_FUNCTION(&Engine::OnMouseMoved, this));
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

bool Engine::OnMouseMoved(MouseMoved& e)
{
	mousePos = { e.GetHorizontalPosition(), e.GetVerticalPosition() };
	return true;
}