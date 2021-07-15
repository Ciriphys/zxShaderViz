#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class ImGui_Frame
{
public: 
	ImGui_Frame();

	void Begin();
	void End();

	void Update(float deltaTime);
	void Finalize();

protected:
	void Init();
	void SetWindowStyle();
};

class UIFrame
{
public:
	virtual void DrawUI() = 0;
};

class MenuBarFrame : public UIFrame
{
	virtual void DrawUI() 
	{ 
		ImGui::Begin("My new window!");
		ImGui::Text("Hello everybody!");
		ImGui::End();
	}
};