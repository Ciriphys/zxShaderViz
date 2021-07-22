#pragma once

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Renderer.h>
#include <Window.h>

#include "ImGuiPanel.h"
#include "Panels.h"

struct EngineSettings
{
	// Theme settings
	int theme;							// 0 dark 1 light 2 blue thing

	// Environment settings
	std::string uResolutionName;		// resolution uniform name
	std::string uMousePositionName;		// mouse position uniform name
	std::string uTimeName;				// time uniform name

	bool hotReload;						// hot reload will Recompile a function on saving
	bool autoSaving;					// if autoSaving is true it will save temporary data 
										// to prevent losses in case of closing without saving

	std::string tempFilepath;			// the location where the temp file will be stored 

};

class Engine
{
public:
	Engine();
	~Engine() = default;

	void RenderLoop();

	static Engine& GetEngineInstance();

	std::shared_ptr<Window> GetWindow() const { return mWindow; }
	std::shared_ptr<EngineSettings> GetSettings() const { return mSettings; }
	std::unordered_map<std::string, UIPanel*> GetUIFrames() const { return mUIFrames; }

	void OpenFile(const std::string& filepath, bool recache = false);
	void SaveFile(const std::string& fragSource, const std::string& filepath = "");
	void CloseFile();

	void Close();

private: 
	void OnEvent(Event&);
	bool OnWindowClosed(WindowClosed&);
	bool OnFilesDropped(FilesDropped&);
	bool OnWindowResized(WindowResized&);

	WindowSettings InitializeSettings(const std::string& filename = "");
	void WriteToConfigFile();

	std::shared_ptr<Window> mWindow;
	std::shared_ptr<Renderer> mRenderer;
	std::unique_ptr<ImGuiPanel> mImGuiFrame;

	static Engine* s_Instance;

	std::unordered_map<std::string, UIPanel*> mUIFrames;

	std::shared_ptr<EngineSettings> mSettings;

	bool mMinimized;
	bool mRunning;
	float lastFrameTime = 0.0f;
};