#include "sppch.h"

#ifndef SP_WIN
#error ShaderPlayground only supports Windows!
#else

#include <Engine.h>

int main()
{
	auto& engine = Engine::GetEngineInstance();
	engine.RenderLoop();

	return 0;
}

	#if defined(SP_DIST) || defined(SP_RELEASE)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	main();
}

	#endif
#endif