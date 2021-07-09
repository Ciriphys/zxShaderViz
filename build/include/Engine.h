#pragma once

#include <Window.h>

class KeyPressed;
class WindowClosed;
class WindowResized;

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
	bool OnWindowResized(WindowResized&);

	std::shared_ptr<Window> m_Window;
	static std::shared_ptr<Engine> s_Instance;

	bool m_Minimized;
};