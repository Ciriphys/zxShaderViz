#include "sppch.h"

#ifndef SP_WIN
#error ShaderPlayground only supports Windows!
#else

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <WindowEventsDispatcher.h>

int main(int argc, char** argv)
{
	//window->SetEventCallbackProcedure(BIND_FUNCTION(WindowEventProcedure));

	while (window->IsActive())
	{
		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		window->Update();
	}

	return 0;
}
#endif