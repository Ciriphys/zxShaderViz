#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class ImGuiPanel
{
public: 
	ImGuiPanel();

	void Begin();
	void End();

	void Update(float deltaTime);
	void Finalize();

	void ChangeTheme(int theme);

protected:
	void Init();
};

