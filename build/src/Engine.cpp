#include "sppch.h"

#include <Engine.h>
#include <FrameBuffer.h>
#include <Utils/TimeStep.h>

#include "Yaml-cpp/yaml.h"

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
	s_Instance = this;

	mWindow = std::make_shared<Window>();
	mWindow->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));
	mImGuiFrame = std::make_unique<ImGuiPanel>();

	mRenderer = Renderer::GetRenderer();

	mMinimized = (mWindow->GetWidth() == 0 && mWindow->GetHeight() == 0);
	mRunning = mWindow->IsActive();

	auto demoPan = new DemoPanel("Demo Panel");
	auto menuBar = new MenuBarPanel("Menu Bar");
	auto viewport = new ViewportPanel("Viewport");
	auto dirPanel = new DirectoryExplorerPanel("Directory Explorer Panel");

	mUIFrames[menuBar->GetName()] = menuBar;
	mUIFrames[demoPan->GetName()] = demoPan;
	mUIFrames[dirPanel->GetName()] = dirPanel;
	mUIFrames[viewport->GetName()] = viewport;
}

void Engine::RenderLoop()
{
	TimeStep ts;

	while (mRunning)
	{
		mWindow->Update();
		if (mMinimized || !mWindow->IsActive()) continue;

		mWindow->Clear();
		ts.Update();

		reinterpret_cast<ViewportPanel*>(mUIFrames["Viewport"])->SetFrameBuffer(mRenderer->mFrameBuffer);

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

	mRenderer->DeleteShaderCache();
	mWindow->Close();
}

Engine& Engine::GetEngineInstance()
{
	if (!s_Instance) new Engine();
	return *s_Instance;
}

void Engine::OpenFile(const std::string& filepath, bool recache)
{
	if (mRenderer->LoadShaderFromFile(filepath, recache))
	{
		mUIFrames["Shader Editor"] = new ShaderEditorPanel("Shader Editor");
		mWindow->SetTitle("ZeXo Shading | " + filepath);
	}
}

void Engine::SaveFile(const std::vector<std::string>& sources, const std::string& filepath)
{
	std::string path;
	path = filepath == "" ? mRenderer->mActiveShader->GetFilepath() : filepath;

	// Get file extension
	std::string extension = {};
	for (auto idx = path.find_last_of("."); idx < path.size(); idx++)
		extension += path[idx];

	ShaderFileType type = GetTypeFromExtension(extension);
	std::fstream file(path, std::fstream::in | std::fstream::out | std::fstream::trunc);

	switch (type)
	{
	case ShaderFileType::GLSL:
		if (!file)
		{
			std::cout << "Could not open file!\n\n";
			return;
		}

		std::cout << "@vertex\n" << sources[0].c_str() << "\n@fragment\n" << sources[1].c_str() << "\n";
		file << "@vertex\n" << sources[0].c_str() << "\n@fragment\n" << sources[1].c_str() << "\n";
		break;
	case ShaderFileType::zxshad:
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Vertex Shader";
		emitter << YAML::Value << sources[0];
		emitter << YAML::Key << "Fragment Shader";
		emitter << YAML::Value << sources[1];

		std::cout << emitter.c_str();
		file << emitter.c_str();
		break;
	}

	file.close();
	OpenFile(path, true);
}

void Engine::CloseFile()
{
	mRenderer->mActiveShader = nullptr;
	mUIFrames.erase("Shader Editor");
}

void Engine::Close()
{
	mRunning = false;
}

void Engine::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Emit<WindowResized>(BIND_FUNCTION(&Engine::OnWindowResized, this));
	dispatcher.Emit<WindowClosed>(BIND_FUNCTION(&Engine::OnWindowClosed, this));
	dispatcher.Emit<FilesDropped>(BIND_FUNCTION(&Engine::OnFilesDropped, this));
	dispatcher.Emit<MouseMoved>(BIND_FUNCTION(&Engine::OnMouseMoved, this));

	if(!e.IsHandled())
	{
		for (auto& [key, panel] : mUIFrames)
			panel->OnEvent(e);
	}
}

bool Engine::OnWindowClosed(WindowClosed& e)
{
	Close();
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
	//if(reinterpret_cast<ViewportPanel*>(mUIFrames["Viewport"])->IsHovered())
	mousePos = { e.GetHorizontalPosition(), e.GetVerticalPosition() };
	return !true;
}

void Engine::SaveGLSL(std::fstream& file, const std::vector<std::string>& sources)
{
	
	return;
}

void Engine::SaveZXSHAD(std::fstream& file, const std::vector<std::string>& sources)
{

	return;
}
