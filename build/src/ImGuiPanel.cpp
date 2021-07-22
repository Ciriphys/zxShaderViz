#include "sppch.h"
#include "ImGuiPanel.h"

#include "Engine.h"
#include "Utils/Input.h"
#include "Utils/FileDialogs.h"

#include <GLFW/glfw3.h>

ImGuiPanel::ImGuiPanel()
{
	Init();
}

void ImGuiPanel::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiPanel::End()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(Engine::GetEngineInstance().GetWindow()->GetWidth(), Engine::GetEngineInstance().GetWindow()->GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiPanel::Update(float deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime > 0.0f ? deltaTime : (float)(1.0f / 60.0f);
}

void ImGuiPanel::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;    
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF("assets/font/OpenSans/OpenSans-regular.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("assets/font/OpenSans/OpenSans-regular.ttf", 34.0f);
	io.Fonts->AddFontFromFileTTF("assets/font/FiraCode/FiraCode-Retina.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("assets/font/Lato/Lato-Regular.ttf", 18.0f);


	ImGui::StyleColorsDark();
	SetWindowStyle();


	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto& app = Engine::GetEngineInstance();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiPanel::SetWindowStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.68f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.29f, 0.29f, 0.29f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.23f, 0.23f, 0.23f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.73f, 0.75f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.68f, 0.70f, 0.72f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.23f, 0.23f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.13f, 0.13f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.53f, 0.53f, 0.53f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.35f, 0.35f, 0.35f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
}

void ImGuiPanel::SetGayStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.34f, 0.32f, 0.32f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.26f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.60f, 0.22f, 0.55f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.89f, 0.62f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.47f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.14f, 0.13f, 0.13f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.52f, 0.48f, 0.53f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.26f, 0.75f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.23f, 0.66f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.97f, 0.16f, 0.87f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.95f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.69f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.87f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.58f, 0.26f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.26f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.73f, 0.26f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.26f, 0.26f, 0.30f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.51f, 0.10f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.10f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.86f, 0.26f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.89f, 0.18f, 0.96f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.94f, 0.10f, 0.97f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.42f, 0.18f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.26f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.20f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 0.75f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

}

void ImGuiPanel::Finalize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

