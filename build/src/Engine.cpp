#include "sppch.h"

#include <Engine.h>
#include <FrameBuffer.h>
#include <Utils/TimeStep.h>

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
	s_Instance = this;

	mWindow = std::make_shared<Window>();
	mWindow->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));
	mImGuiFrame = std::make_unique<ImGui_Frame>();

	mRenderer = Renderer::GetRenderer();

	mMinimized = (mWindow->GetWidth() == 0 && mWindow->GetHeight() == 0);

	auto menuBar = new MenuBarFrame("Menu Bar");
	auto viewport = new ViewportFrame("Viewport");

	mUIFrames[menuBar ->GetName()] = menuBar;
	mUIFrames[viewport->GetName()] = viewport;
}

void Engine::RenderLoop()
{
	TimeStep ts;

	while (mWindow->IsActive())
	{
		mWindow->Update();
		if (mMinimized || !mWindow->IsActive()) continue;

		mWindow->Clear();
		ts.Update();

		reinterpret_cast<ViewportFrame*>(mUIFrames["Viewport"])->SetFrameBuffer(mRenderer->mFrameBuffer);

		mImGuiFrame->Begin();
		for (auto& [k, uiframe] : mUIFrames)
			uiframe->DrawUI();
		mImGuiFrame->End();

		if (!mRenderer->mActiveShader) continue;

		mRenderer->mActiveShader->SetUniform("u_Time", ts.GetExecutionTimef());	
		mRenderer->mActiveShader->SetUniform("u_MousePos", mousePos);
		mRenderer->mActiveShader->SetUniform("u_Resolution", { mWindow->GetWidth(), mWindow->GetHeight() });
		mRenderer->Draw();
	}
}

Engine& Engine::GetEngineInstance()
{
	if (!s_Instance) new Engine();
	return *s_Instance;
}

void Engine::OpenFile(const std::string& filepath, bool recache)
{
	if (mRenderer->LoadShaderFromGLSLPath(filepath, recache))
		mUIFrames["Shader Editor"] = new ShaderEditorFrame("Shader Editor");
}

void Engine::SaveFile(const std::vector<std::string>& sources)
{
	auto path = mRenderer->mActiveShader->GetFilepath();
	std::fstream file(path);
	//file.open(, std::ios::trunc);
	if (!file)
	{
		std::cout << "Could not open file!\n\n";
		return;
	}
	file << "@vertex\n" << sources[0] << "\n@fragment\n" << sources[1];
	file.close();

	OpenFile(path, true);
}

void Engine::CloseFile()
{
	mRenderer->mActiveShader = nullptr;
	mUIFrames.erase("Shader Editor");
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
	if (e.GetKeyCode() == 48)
		mRenderer->DeleteShaderCache();

	return false;
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
		this->OpenFile(files);

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