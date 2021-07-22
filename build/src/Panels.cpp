#include "sppch.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Engine.h"
#include "Panels.h"

#include "Utils/Input.h"
#include "Utils/FileDialogs.h"

 //////////////////////////
////  VIEWPORT PANEL  ////
/////////////////////////
void ViewportPanel::DrawUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::Begin("Viewport", &mActive, windowFlags);
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

			if (ImGui::IsWindowHovered())
			{
				ImVec2 mPos = ImGui::GetMousePos();
				mMousePos = { mPos.x, mPos.y };
			}
		}
		else
		{
			auto& io = ImGui::GetIO();
			auto font = io.Fonts->Fonts[1];
			ImGui::PushFont(font);

			std::string text = "The shader couldn't be compiled!\nCheck your source code!";

			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
			auto textHeight = ImGui::CalcTextSize(text.c_str()).y;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
			ImGui::TextWrapped(text.c_str());

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
		ImGui::TextWrapped(text.c_str());

		ImGui::PopFont();
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

  ///////////////////////////////
 ////  SHADER EDITOR PANEL  ////
///////////////////////////////
void ShaderEditorPanel::DrawUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
	
	auto& io = ImGui::GetIO();
	auto font = io.Fonts->Fonts[2];
	ImGui::PushFont(font);

	ImGui::Begin("Shader Editor", &mActive);
	if (auto shader = Renderer::GetRenderer()->GetShader())
	{
		if (mFragmentSrc == "")
			mFragmentSrc = shader->GetFragmentSource();

		ImGui::InputTextMultiline("Fragment",
			(char*)mFragmentSrc.c_str(),
			mFragmentSrc.capacity() + 1,
			ImGui::GetWindowSize(), 
			ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackResize, 
			[](ImGuiInputTextCallbackData* data) -> int
			{
				if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
				{
					std::string* str = (std::string*)data->UserData;
					IM_ASSERT(data->Buf == str->c_str());
					str->resize(data->BufTextLen);
					data->Buf = (char*)str->c_str();
				}

				return 0;
			},
			reinterpret_cast<void*>(&mFragmentSrc)
		);
	}
	ImGui::End();

	ImGui::PopFont();
	ImGui::PopStyleVar(2);
}

  //////////////////////////
 ////  MENU BAR PANEL  ////
//////////////////////////
void MenuBarPanel::DrawUI()
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoCloseButton;

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
					auto fragSource = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetFragSource();
					Engine::GetEngineInstance().SaveFile(fragSource);
				}
			}

			if (ImGui::MenuItem("Save As...", "Ctrl+Alt+S", nullptr, Renderer::GetRenderer()->GetShader().get()))
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto fragSource = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetFragSource();
					Engine::GetEngineInstance().SaveFile(fragSource, FileDialogs::SaveFileAs(""));
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
			std::string label = (Engine::GetEngineInstance().GetWindow()->IsFullscreen() ? "Exit" : "Enter");
			label.append(" Full Screen");
			if (ImGui::MenuItem(label.c_str(), "F11"))
			{
				Engine::GetEngineInstance().GetWindow()->SetFullscreen(!Engine::GetEngineInstance().GetWindow()->IsFullscreen());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Editor Preferences", nullptr, nullptr))
			{
				auto& engineInstance = Engine::GetEngineInstance();
				auto epPanel = reinterpret_cast<EditorPreferencesPanel*>(engineInstance.GetUIFrames()["Editor Preferences"]);

				epPanel->Activate(true);
			}
		
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Shader", Renderer::GetRenderer()->GetShader().get()))
		{
			if (ImGui::MenuItem("Recompile Shader", "Ctrl+F5", nullptr, !Engine::GetEngineInstance().GetSettings()->hotReload))
			{
				auto& engine = Engine::GetEngineInstance();
				auto filepath = Renderer::GetRenderer()->GetShader()->GetFilepath();

				engine.OpenFile(filepath, true);
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Delete Cache"))
				Renderer::GetRenderer()->DeleteShaderCache();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			for (auto [name, panel] : Engine::GetEngineInstance().GetUIFrames())
				if (name != "Editor Preferences" && name != "Menu Bar")
					if (ImGui::MenuItem(name.c_str(), nullptr, false, !panel->IsActive()))
						panel->Activate(true);

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
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetFragSource();
					Engine::GetEngineInstance().SaveFile(sources, FileDialogs::SaveFileAs(""));
				}
			}
			else if (control)
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetFragSource();
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
		case Key::F11:
			Engine::GetEngineInstance().GetWindow()->SetFullscreen(!Engine::GetEngineInstance().GetWindow()->IsFullscreen());
			break;
		}

		return true;
		});
}

  /////////////////////
 ////  LOG PANEL  ////
/////////////////////
void LogPanel::DrawUI()
{
	std::stringstream msgData;
	for (auto it = mLogMsgs.rbegin(); it != mLogMsgs.rend(); ++it)
		msgData << *it;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });
	ImGui::Begin("Output Logger", &mActive, windowFlags);
	if (ImGui::Button("Clear")) mLogMsgs.clear();
	ImGui::SameLine();
	if (ImGui::Button("Copy to Clipboard"))
	{
		std::stringstream clipboard;
		for (auto& msg : mLogMsgs)
			clipboard << msg;
		
		ImGui::SetClipboardText(clipboard.str().c_str());
	}

	auto& io = ImGui::GetIO();
	auto font = io.Fonts->Fonts[3];
	ImGui::PushFont(font);

	ImGui::InputTextMultiline("Logger", (char*)msgData.str().c_str(), msgData.str().size(), ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
	ImGui::PopFont();

	ImGui::End();
	ImGui::PopStyleVar();
}

void LogPanel::PushMessage(const std::string& msg)
{
	mLogMsgs.push_back(msg);
}

EditorPreferencesPanel::EditorPreferencesPanel(const std::string& name) : UIPanel(name, false)
{
	windowFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
}

void EditorPreferencesPanel::DrawUI()
{
	auto settings = Engine::GetEngineInstance().GetSettings();

	if (mActive && !ImGui::IsPopupOpen("Editor Preferences"))
		ImGui::OpenPopup("Editor Preferences");

	if (ImGui::BeginPopupModal("Editor Preferences", &mActive, windowFlags))
	{
		ImGui::SetWindowSize({ 500, 400 });
		if (ImGui::CollapsingHeader("Theme"))
		{
			if (ImGui::Combo("Selected Theme", &settings->theme, "Dark\0Light\0"))
			{
				switch (settings->theme)
				{
				case 0:
					break;
				case 1:
					break;
				}
			}
		}

		if (ImGui::CollapsingHeader("Environment"))
		{
			ImGui::Text("Uniform Names");
			ImGui::InputText("Resolution", settings->uResolutionName.data(), 25);
			ImGui::InputText("Mouse Position", settings->uMousePositionName.data(), 25);
			ImGui::InputText("Time", settings->uTimeName.data(), 25);
			ImGui::Separator();

			ImGui::Text("Shader");
			ImGui::Checkbox("Hot reload", &settings->hotReload);
			ImGui::Checkbox("Auto saving", &settings->autoSaving);

			if (settings->autoSaving)
			{
				auto file = (settings->tempFilepath == "" ? "<default file>" : settings->tempFilepath.c_str());
				ImGui::Text("%s", file);
				ImGui::SameLine();
				ImGui::Button("Select a file");
			}
		}

		ImGui::EndPopup();
	}
}
