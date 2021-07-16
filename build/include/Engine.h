#pragma once

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Renderer.h>
#include <Window.h>

#include "ImGui_Frame.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void RenderLoop();

	static Engine& GetEngineInstance();
	std::shared_ptr<Window> GetWindow() const { return mWindow; }

private: 
	void OnEvent(Event&);
	bool OnKeyPressed(KeyPressed&);
	bool OnWindowClosed(WindowClosed&);
	bool OnFilesDropped(FilesDropped&);
	bool OnWindowResized(WindowResized&);
	bool OnMouseMoved(MouseMoved&);

	std::shared_ptr<Window> mWindow;
	std::shared_ptr<Renderer> mRenderer;
	std::unique_ptr<ImGui_Frame> mImGuiFrame;

	static Engine* s_Instance;

	std::vector<UIFrame*> mUIFrames;

	bool mMinimized;
	float lastFrameTime = 0.0f;

	glm::vec2 mousePos = {};
};