#include "sppch.h"
#include "ImGui_Frame.h"

#include "Engine.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

std::string OpenFile(const char * filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Engine::GetEngineInstance().GetWindow()->GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();

}

std::string SaveFileAs(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Engine::GetEngineInstance().GetWindow()->GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

ImGui_Frame::ImGui_Frame()
{
	Init();
}

void ImGui_Frame::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGui_Frame::End()
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

void ImGui_Frame::Update(float deltaTime)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = deltaTime > 0.0f ? deltaTime : (float)(1.0f / 60.0f);
}

void ImGui_Frame::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF("assets/font/OpenSans-regular.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("assets/font/OpenSans-regular.ttf", 34.0f);

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

void ImGui_Frame::SetWindowStyle()
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

void ImGui_Frame::Finalize()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ViewportFrame::DrawUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport");
	if (auto shader = Renderer::GetRenderer()->GetShader())
	{
		if (shader->IsLinked())
		{
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (mViewportSize != (*(glm::vec2*)&viewportSize))
			{
				mFrameBuffer->Resize((unsigned int)viewportSize.x, (unsigned int)viewportSize.y);
				mViewportSize = { viewportSize.x, viewportSize.y };
			}
			uint32_t textureID = mFrameBuffer->GetColorAttachment();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
		else
		{
			auto& io = ImGui::GetIO();
			auto font = io.Fonts->Fonts[1];
			ImGui::PushFont(font);

			std::string text = "The shader couldn't be compiled!\n\t\tCheck your source code!";

			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
			auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
			ImGui::Text(text.c_str());

			ImGui::PopFont();
		}
	}
	else
	{	
		auto& io = ImGui::GetIO();
		auto font = io.Fonts->Fonts[1];
		ImGui::PushFont(font);

		std::string text = "Drop a glsl file or Click Open (Ctrl+O) to portray it!";

		auto windowWidth = ImGui::GetWindowSize().x;
		auto windowHeight = ImGui::GetWindowSize().y;
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
		auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
		ImGui::Text(text.c_str());

		ImGui::PopFont();
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void ShaderEditorFrame::DrawUI()
{
	ImGui::Begin("Shader Editor");
	if (auto shader = Renderer::GetRenderer()->GetShader())
	{
		if (mVertexSrc == "" && mFragmentSrc == "")
		{
			mVertexSrc = shader->operator[](0);
			mFragmentSrc = shader->operator[](1);
		}

		auto& io = ImGui::GetIO();
		auto sz = io.Fonts->Fonts[0]->FontSize + 5;
		
		const int shaderCount = 2;
		auto windowWidth  = ImGui::GetWindowSize().x;
		auto windowHeight = ImGui::GetWindowSize().y / shaderCount - 2 * sz;

		ImGui::Text("Vertex Source");
		ImGui::InputTextMultiline("Vertex Source", mVertexSrc.data(), mVertexSrc.capacity(), ImVec2{windowWidth - 15, windowHeight });
		ImGui::Separator();
		ImGui::Text("Fragment Source");
		ImGui::InputTextMultiline("Fragment Source", mFragmentSrc.data(), mFragmentSrc.capacity(), ImVec2{ windowWidth - 15, windowHeight });
	}
	ImGui::End();
}

void MenuBarFrame::DrawUI()
{
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("MenuBar", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("MenuBarDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	//static bool shouldOpen = false, shouldSave = false;
	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New", "Ctrl+N", nullptr);
			if (ImGui::MenuItem("Open", "Ctrl+O", nullptr))
			{
				Engine::GetEngineInstance().OpenFile(OpenFile(""));
			}
			if (ImGui::MenuItem("Save", "Ctrl+S", nullptr))
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorFrame*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetSources();
					Engine::GetEngineInstance().SaveFile(sources);
				}
			}

			ImGui::Separator();
			if(ImGui::MenuItem("Close File", "Shift+C", nullptr))
			{
				Engine::GetEngineInstance().CloseFile();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Options", nullptr, nullptr);
			ImGui::MenuItem("Blueprint", nullptr, nullptr);
			ImGui::MenuItem("Another opt", nullptr, nullptr);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("?"))
		{
			ImGui::MenuItem("About", "Ctrl+H", nullptr);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}
