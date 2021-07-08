#include "sppch.h"

#include <WindowEventsDispatcher.h>

bool OnWindowClosed(WindowClosed& e)
{
	window->Close();
	return true;
}

bool OnKeyPressed(KeyPressed& e)
{
	window->SetTitle(std::to_string(e.GetKeyCode()));
	return true;
}

void WindowEventProcedure(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Emit<WindowClosed>(BIND_FUNCTION(OnWindowClosed));
	dispatcher.Emit<KeyPressed>(BIND_FUNCTION(OnKeyPressed));
}


