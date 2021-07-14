#include "sppch.h"

#include <Engine.h>
#include <Utils/TimeStep.h>

std::shared_ptr<Engine> Engine::s_Instance = nullptr;

Engine::Engine()
{
	mWindow = std::make_shared<Window>();
	mWindow->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));

	mRenderer = Renderer::GetRenderer();

	mMinimized = (mWindow->GetWidth() == 0 && mWindow->GetHeight() == 0);
}

void Engine::RenderLoop()
{
	TimeStep ts;

	while (mWindow->IsActive())
	{
		mWindow->Update();
		if (mMinimized) continue;

		ts.Update();

		mWindow->Clear();
		if (!mRenderer->GetShader()) continue;

		mRenderer->GetShader()->SetUniform("u_Time", ts.GetExecutionTimef());	
		mRenderer->GetShader()->SetUniform("u_MousePos", mousePos);
		mRenderer->GetShader()->SetUniform("u_Resolution", { mWindow->GetWidth(), mWindow->GetHeight() });
		mRenderer->Draw();
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
	dispatcher.Emit<FilesDropped>(BIND_FUNCTION(&Engine::OnFilesDropped, this));
	dispatcher.Emit<KeyPressed>(BIND_FUNCTION(&Engine::OnKeyPressed, this));
	dispatcher.Emit<MouseMoved>(BIND_FUNCTION(&Engine::OnMouseMoved, this));
}

bool Engine::OnKeyPressed(KeyPressed& e)
{
	if (e.GetKeyCode() == 65)
		mRenderer->DeleteShaderCache();

	return true;
}

bool Engine::OnWindowClosed(WindowClosed& e)
{
	mRenderer->DeleteShaderCache();
	mWindow->Close();
	return true;
}

bool Engine::OnFilesDropped(FilesDropped& e)
{
	for (auto& files : e.GetFiles())
		mRenderer->LoadShaderFromGLSLPath(files);

	return true;
}

bool Engine::OnWindowResized(WindowResized& e)
{
	mMinimized = (e.GetWidth() == 0 && e.GetHeigth() == 0);
	return true;
}

bool Engine::OnMouseMoved(MouseMoved& e)
{
	mousePos = { e.GetHorizontalPosition(), e.GetVerticalPosition() };
	return true;
}