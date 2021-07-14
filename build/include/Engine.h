#pragma once

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Renderer.h>
#include <Window.h>

class Engine
{
public:
	Engine();
	~Engine() = default;

	void RenderLoop();

	static std::shared_ptr<Engine> GetEngineInstance();

private: 
	void OnEvent(Event&);
	bool OnKeyPressed(KeyPressed&);
	bool OnWindowClosed(WindowClosed&);
	bool OnFilesDropped(FilesDropped&);
	bool OnWindowResized(WindowResized&);
	bool OnMouseMoved(MouseMoved&);

	std::shared_ptr<Window> mWindow;
	std::shared_ptr<Renderer> mRenderer;
	static std::shared_ptr<Engine> s_Instance;

	bool mMinimized;
	float lastFrameTime = 0.0f;

	glm::vec2 mousePos = {};
};