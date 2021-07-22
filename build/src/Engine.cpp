#include "sppch.h"

#include <Engine.h>
#include <FrameBuffer.h>
#include <Utils/TimeStep.h>

#include "Yaml-cpp/yaml.h"

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
	s_Instance = this;
	WindowSettings windowSettings;

	if (std::filesystem::exists("config.cfg"))
	{
		windowSettings = InitializeSettings("config.cfg");
	}
	else
	{
		windowSettings = InitializeSettings();
		WriteToConfigFile();
	}

	mWindow = std::make_shared<Window>(windowSettings);
	mWindow->SetEventCallbackProcedure(BIND_FUNCTION(&Engine::OnEvent, this));
	mImGuiFrame = std::make_unique<ImGuiPanel>();

	mRenderer = Renderer::GetRenderer();

	mMinimized = (mWindow->GetWidth() == 0 && mWindow->GetHeight() == 0);
	mRunning = mWindow->IsActive();

	auto demoPan = new DemoPanel("Demo Panel");
	auto menuBar = new MenuBarPanel("Menu Bar");
	auto viewport = new ViewportPanel("Viewport");
	auto logPanel = new LogPanel("Log Panel");
	auto epPanel = new EditorPreferencesPanel("Editor Preferences");
	auto sePanel = new ShaderEditorPanel("Shader Editor");

	mUIFrames[menuBar->GetName()] = menuBar;
	mUIFrames[demoPan->GetName()] = demoPan;
	mUIFrames[epPanel->GetName()] = epPanel;
	mUIFrames[sePanel->GetName()] = sePanel;
	mUIFrames[viewport->GetName()] = viewport;
	mUIFrames[logPanel->GetName()] = logPanel;
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
			if(uiframe->IsActive())
				uiframe->DrawUI();
		mImGuiFrame->End();

		if (!mRenderer->mActiveShader) continue;

		mRenderer->mActiveShader->SetUniform(mSettings->uResolutionName, { mWindow->GetWidth(), mWindow->GetHeight() });
		mRenderer->mActiveShader->SetUniform(mSettings->uMousePositionName, reinterpret_cast<ViewportPanel*>(mUIFrames["Viewport"])->GetMousePos());
		mRenderer->mActiveShader->SetUniform(mSettings->uTimeName, ts.GetExecutionTimef());	
		mRenderer->Draw();
	}

	WriteToConfigFile();

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
		mWindow->SetTitle("ZeXo Shading | " + filepath);
	}
}

void Engine::SaveFile(const std::string& fragSource, const std::string& filepath)
{
	std::string path;
	path = filepath == "" ? mRenderer->mActiveShader->GetFilepath() : filepath;

	// Get file extension
	std::string extension = {};
	for (auto idx = path.find_last_of("."); idx < path.size(); idx++)
		extension += path[idx];

	std::transform(extension.begin(), extension.end(), extension.begin(), [](char c) -> char { return std::tolower(c); });

	ShaderFileType type = GetTypeFromExtension(extension);
	std::fstream file(path, std::fstream::in | std::fstream::out | std::fstream::trunc);

	if (!file)
	{
		std::stringstream msg;
		msg << "Could not open file!\n\n";

		reinterpret_cast<LogPanel*>(Engine::GetEngineInstance().GetUIFrames()["Log Panel"])->PushMessage(msg.str());

		return;
	}

	switch (type)
	{
	case ShaderFileType::frag:
		file << fragSource << '\n';
		break;
	default:
		break;
	}

	file.close();
	if(mSettings->hotReload)
		OpenFile(path, true);
}

void Engine::CloseFile()
{
	mRenderer->mActiveShader = nullptr;
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

WindowSettings Engine::InitializeSettings(const std::string& filename)
{
	mSettings = std::make_shared<EngineSettings>();
	WindowSettings windowSettings = { 1920, 1080, 0, 30, false, (RefreshRate)0 };

	if (filename != "")
	{
		// Deserialize with YAML
		std::fstream file(filename);
		
		if (file)
		{
			std::stringstream fileBuffer;
			fileBuffer << file.rdbuf();
			file.close();

			YAML::Node nodes = YAML::Load(fileBuffer);
			auto engineNode = nodes["Engine"];
			auto windowNode = nodes["Window"];

			// Engine Settings
			if (engineNode)
			{
				auto themeSettings = engineNode["Theme Settings"];
				if (themeSettings)
				{
					mSettings->theme = themeSettings["Theme"].as<int>();
				}
				auto environmentSettings = engineNode["Environment Settings"];
				if (environmentSettings)
				{
					auto uniformSettings = environmentSettings["Uniforms"];
					if (uniformSettings)
					{
						mSettings->uResolutionName = uniformSettings["Resolution"].as<std::string>();
						mSettings->uMousePositionName = uniformSettings["Mouse Position"].as<std::string>();
						mSettings->uTimeName = uniformSettings["Time"].as<std::string>();
					}

					auto shaderSettings = environmentSettings["Shaders"];
					if (shaderSettings)
					{
						mSettings->hotReload = shaderSettings["Hot Reload"].as<bool>();
						mSettings->autoSaving = shaderSettings["Auto Saving"].as<bool>();
						mSettings->tempFilepath = shaderSettings["Temp Filepath"].as<std::string>();
					}
				}
			}

			// Window Settings
			if (windowNode)
			{
				auto windowSize = windowNode["Window Size"];
				if (windowSize)
				{
					windowSettings.windowSizex = windowSize[0].as<int>();
					windowSettings.windowSizey = windowSize[1].as<int>();
				}

				auto windowPos = windowNode["Window Position"];
				if (windowPos)
				{
					windowSettings.windowPosx = windowPos[0].as<int>();
					windowSettings.windowPosy = windowPos[1].as<int>();
				}

				if (auto fullscreen = windowNode["Fullscreen"])
				{
					windowSettings.fullscreen = fullscreen.as<bool>();
				}

				if (auto refreshRate = windowNode["Refresh Rate"])
				{
					windowSettings.refreshRate = (RefreshRate)refreshRate.as<unsigned int>();
				}
			}

			return windowSettings;
		}
	}

	// Theme settings
	mSettings->theme = 0;

	// Environment
	mSettings->uResolutionName = "uResolution";
	mSettings->uMousePositionName = "uMousePosition";
	mSettings->uTimeName = "uTime";
	mSettings->hotReload = true;
	mSettings->autoSaving = true;
	mSettings->tempFilepath = "";

	// Default Window Settings
	return windowSettings;
}

void Engine::WriteToConfigFile()
{
	std::fstream file("config.cfg", std::fstream::in | std::fstream::out | std::fstream::trunc);
	YAML::Emitter emitter;

	// Config
	{
		emitter << YAML::Comment("ZeXo Playground - v1.0.0 Alpha");
		emitter << YAML::BeginMap;

		// Engine settings
		emitter << YAML::Key << "Engine";
		{
			emitter << YAML::BeginMap;

			// Theme settings
			emitter << YAML::Key << "Theme Settings";
			{
				emitter << YAML::BeginMap;
				emitter << YAML::Key << "Theme" << YAML::Value << mSettings->theme;
				emitter << YAML::EndMap;
			}

			// Environment settings
			emitter << YAML::Key << "Environment Settings";
			{
				emitter << YAML::BeginMap;
				// Uniforms
				emitter << YAML::Key << "Uniforms";
				{
					emitter << YAML::BeginMap;
					emitter << YAML::Key << "Resolution" << YAML::Value << mSettings->uResolutionName;
					emitter << YAML::Key << "Mouse Position" << YAML::Value << mSettings->uMousePositionName;
					emitter << YAML::Key << "Time" << YAML::Value << mSettings->uTimeName;
					emitter << YAML::EndMap;
				}

				// Shaders settings
				emitter << YAML::Key << "Shaders";
				{
					emitter << YAML::BeginMap;
					emitter << YAML::Key << "Hot Reload" << YAML::Value << mSettings->hotReload;
					emitter << YAML::Key << "Auto Saving" << YAML::Value << mSettings->autoSaving;
					emitter << YAML::Key << "Temp Filepath" << YAML::Value << mSettings->tempFilepath;
					emitter << YAML::EndMap;
				}

				emitter << YAML::EndMap;
			}

			emitter << YAML::EndMap;
		}

		// Window settings
		emitter << YAML::Key << "Window";
		{
			std::vector<int> wSize = { 1028, 720 };
			std::vector<int> wPos = { 0, 0 };
			bool fs = false;
			RefreshRate rr = RefreshRate_Unlimited;

			if (mWindow)
			{
				wSize[0] = mWindow->GetWidth();
				wSize[1] = mWindow->GetHeight();

				wPos[0] = mWindow->GetPosX();
				wPos[1] = mWindow->GetPosY();

				fs = mWindow->IsFullscreen();
				rr = mWindow->GetRefreshRate();
			}

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Window Size" << YAML::Value << wSize;
			emitter << YAML::Key << "Window Position" << YAML::Value << wPos;
			emitter << YAML::Key << "Fullscreen" << YAML::Value << fs;
			emitter << YAML::Key << "Refresh Rate" << YAML::Value << rr;
			emitter << YAML::EndMap;
		}
	
		emitter << YAML::EndMap;
	}

	file << emitter.c_str();
	file.close();

	return;
}
