#include "sppch.h"

#include "imgui.h"
#include "Panels.h"
#include "Engine.h"

#include "Utils/Input.h"
#include "Utils/FileDialogs.h"

void ViewportPanel::DrawUI()
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

void ShaderEditorPanel::DrawUI()
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
		auto windowWidth = ImGui::GetWindowSize().x;
		auto windowHeight = ImGui::GetWindowSize().y / shaderCount - 2 * sz;

		ImGui::Text("Vertex Source");
		ImGui::InputTextMultiline("Vertex Source", mVertexSrc.data(), mVertexSrc.size(), ImVec2{ windowWidth - 15, windowHeight });
		ImGui::Separator();
		ImGui::Text("Fragment Source");
		ImGui::InputTextMultiline("Fragment Source", mFragmentSrc.data(), mFragmentSrc.size(), ImVec2{ windowWidth - 15, windowHeight });
	}

	ImGui::End();
}

void MenuBarPanel::DrawUI()
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
				Engine::GetEngineInstance().OpenFile(FileDialogs::OpenFile(""));
			}

			if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, Renderer::GetRenderer()->GetShader().get()))
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetSources();
					Engine::GetEngineInstance().SaveFile(sources);
				}
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Alt+S", nullptr, Renderer::GetRenderer()->GetShader().get()))
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetSources();
					Engine::GetEngineInstance().SaveFile(sources, FileDialogs::SaveFileAs(""));
				}
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Close File", "Shift+C", nullptr, Renderer::GetRenderer()->GetShader().get()))
			{
				Engine::GetEngineInstance().CloseFile();
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4", nullptr))
			{
				Engine::GetEngineInstance().Close();
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

void MenuBarPanel::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Emit<KeyPressed>([](KeyPressed& e) -> bool {

		Key keycode = (Key)e.GetKeyCode();

		bool shift = Input::GetKeyPress(Key::LeftShift) || Input::GetKeyPress(Key::RightShift);
		bool control = Input::GetKeyPress(Key::LeftControl) || Input::GetKeyPress(Key::RightControl);
		bool alt = Input::GetKeyPress(Key::LeftAlt) || Input::GetKeyPress(Key::RightAlt);

		switch (keycode)
		{
		case Key::O:
			if (control)
				Engine::GetEngineInstance().OpenFile(FileDialogs::OpenFile(""));
			break;
		case Key::S:
			if (control && alt)
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetSources();
					Engine::GetEngineInstance().SaveFile(sources, FileDialogs::SaveFileAs(""));
				}
			}
			else if (control)
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetSources();
					Engine::GetEngineInstance().SaveFile(sources);
				}
			}
			break;

		case Key::C:
			if (shift)
			{
				Engine::GetEngineInstance().CloseFile();
			}
			break;

		case Key::F4:
			if (alt)
			{
				Engine::GetEngineInstance().Close();
			}
			break;
		}

		return true;
		});
}
