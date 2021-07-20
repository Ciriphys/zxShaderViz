#pragma once

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Renderer.h>
#include <Window.h>

#include "ImGuiPanel.h"
#include "Panels.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void RenderLoop();

	static Engine& GetEngineInstance();
	std::shared_ptr<Window> GetWindow() const { return mWindow; }
	std::unordered_map<std::string, UIPanel*> GetUIFrames() const { return mUIFrames; }

	void OpenFile(const std::string& filepath, bool recache = false);
	void SaveFile(const std::vector<std::string>& sources, const std::string& filepath = "");
	void CloseFile();

	void Close();

private: 
	void OnEvent(Event&);
	bool OnWindowClosed(WindowClosed&);
	bool OnFilesDropped(FilesDropped&);
	bool OnWindowResized(WindowResized&);

	std::shared_ptr<Window> mWindow;
	std::shared_ptr<Renderer> mRenderer;
	std::unique_ptr<ImGuiPanel> mImGuiFrame;

	static Engine* s_Instance;

	std::unordered_map<std::string, UIPanel*> mUIFrames;

	bool mMinimized;
	bool mRunning;
	float lastFrameTime = 0.0f;
};