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

	ImGui::End();
	ImGui::PopStyleVar();
}

  ///////////////////////////////
 ////  SHADER EDITOR PANEL  ////
///////////////////////////////
void ShaderEditorPanel::DrawUI()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 3, 3 });
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);

	auto& io = ImGui::GetIO();
	auto font = io.Fonts->Fonts[2];
	ImGui::PushFont(font);
	
	ImGui::Begin("Shader Editor", &mActive, windowFlags);
	if(mFilename != "")
	{
		if (mFragmentSrc == "")
			if (auto shader = Renderer::GetRenderer()->GetShader())
				mFragmentSrc = shader->GetFragmentSource();

		ImGui::BeginChild(ImGui::GetID("Fragment"));
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
					str->resize(data->BufTextLen + 1);
					data->Buf = (char*)str->c_str();
				}

				return 0;
			},
			reinterpret_cast<void*>(&mFragmentSrc)
		);
		ImGui::SetScrollY(ImGui::GetScrollMaxY());
		ImGui::EndChild();

		if (auto shader = Renderer::GetRenderer()->GetShader())
			if (mFragmentSrc != Renderer::GetRenderer()->GetShader()->GetFragmentSource())
			{
				mSaved = false;
				if (Engine::GetEngineInstance().GetWindow()->GetTitle().find('*') == std::string::npos)
					Engine::GetEngineInstance().GetWindow()->SetTitle(Engine::GetEngineInstance().GetWindow()->GetTitle() + '*');

				if (Engine::GetEngineInstance().GetSettings()->autoSaving)
					Engine::GetEngineInstance().WriteToTempFile(mFilename + ' ' + mFragmentSrc);
			}
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

	auto& engineInstance = Engine::GetEngineInstance();
	auto shaderEditorPanel = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"]);
	auto ucPanel = reinterpret_cast<UpdateChangesPanel*>(engineInstance.GetUIFrames()["Update Changes"]);
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("New", "Ctrl+N", nullptr))
			{
				if (shaderEditorPanel->IsSaved())
				{
					Engine::GetEngineInstance().NewFile();
				}
				else
				{
					ucPanel->Activate(true, 0);
				}
			}

			if (ImGui::MenuItem("Open", "Ctrl+O", nullptr))
			{
				if (shaderEditorPanel->IsSaved())
				{
					Engine::GetEngineInstance().OpenFile(FileDialogs::OpenFile(""));
				}
				else
				{
					ucPanel->Activate(true, 1);
				}
			}

			if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, Renderer::GetRenderer()->GetShader().get() || shaderEditorPanel->GetFilename() == "untitled.frag"))
			{
				if (Renderer::GetRenderer()->GetShader())
				{
					auto sources = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"])->GetFragSource();
					Engine::GetEngineInstance().SaveFile(sources);
				}
				else
				{
					auto fragSource = shaderEditorPanel->GetFragSource();
					if (shaderEditorPanel->GetFilename() == "untitled.frag")
					{
						auto result = FileDialogs::SaveFileAs("");
						shaderEditorPanel->SetFilename(result == "" ? "untitled.frag" : result);
						if (result != "")
							Engine::GetEngineInstance().SaveFile(fragSource, shaderEditorPanel->GetFilename());
					}
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
				if (shaderEditorPanel->IsSaved())
					Engine::GetEngineInstance().CloseFile();
				else
					ucPanel->Activate(true, 2);
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
			if (ImGui::MenuItem("Editor Preferences", nullptr, nullptr))
			{
				auto& engineInstance = Engine::GetEngineInstance();
				auto epPanel = reinterpret_cast<EditorPreferencesPanel*>(engineInstance.GetUIFrames()["Editor Preferences"]);

				epPanel->Activate(true);
			}
		
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Shader"))
		{
			if (ImGui::MenuItem("Recompile Shader", "Ctrl+F5", nullptr, !Engine::GetEngineInstance().GetSettings()->hotReload && Renderer::GetRenderer()->GetShader()))
			{
				auto& engine = Engine::GetEngineInstance();
				auto filepath = Renderer::GetRenderer()->GetShader()->GetFilepath();

				engine.OpenFile(filepath, true);
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Delete Cache", nullptr, nullptr, Renderer::GetRenderer()->GetRawShaderCache().size() > 0))
			{
				Engine::GetEngineInstance().CloseFile();
				Renderer::GetRenderer()->DeleteShaderCache();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			for (auto [name, panel] : Engine::GetEngineInstance().GetUIFrames())
				if (name != "Editor Preferences" && name != "Menu Bar" && name != "Update Changes")
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
		case Key::N:
			if (control)
			{
				if (auto shaderEditorPanel = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"]))
				{
					if (shaderEditorPanel->IsSaved())
					{
						Engine::GetEngineInstance().NewFile();
					}
					else
					{
						auto& engineInstance = Engine::GetEngineInstance();
						auto ucPanel = reinterpret_cast<UpdateChangesPanel*>(engineInstance.GetUIFrames()["Update Changes"]);

						ucPanel->Activate(true, 0);
					}
				}
			}
			break;
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
				else
				{
					auto se = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"]);
					auto fragSource = se->GetFragSource();
					if (se->GetFilename() == "untitled.frag")
					{
						auto result = FileDialogs::SaveFileAs("");
						se->SetFilename(result == "" ? "untitled.frag" : result);
						if (result != "")
							Engine::GetEngineInstance().SaveFile(fragSource, se->GetFilename());
					}
				}
			}
			break;

		case Key::C:
			if (shift)
			{
				auto& engineInstance = Engine::GetEngineInstance();
				auto ucPanel = reinterpret_cast<UpdateChangesPanel*>(engineInstance.GetUIFrames()["Update Changes"]);
				auto se = reinterpret_cast<ShaderEditorPanel*>(Engine::GetEngineInstance().GetUIFrames()["Shader Editor"]);

				if (se->IsSaved())
					Engine::GetEngineInstance().CloseFile();
				else
					ucPanel->Activate(true, 2);
			}
			break;

		case Key::F4:
			if (alt)
			{
				Engine::GetEngineInstance().Close();
			}
			break;
		case Key::F5:
			if (control && !Engine::GetEngineInstance().GetSettings()->hotReload && Renderer::GetRenderer()->GetShader())
			{
				Engine::GetEngineInstance().OpenFile(Renderer::GetRenderer()->GetShader()->GetFilepath(), true);
			}
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
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 5.0f, 5.0f });
	ImGui::Begin("Output Logger", &mActive, windowFlags);
	if (ImGui::Button("Clear")) mLogMsgs.clear();
	ImGui::SameLine();
	if (ImGui::Button("Copy to Clipboard"))
	{
		std::stringstream clipboard;
		for (auto& [msg, severity] : mLogMsgs)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, GetColorFromSeverity(severity));
			clipboard << msg;
			ImGui::PopStyleColor();
		}
		
		ImGui::SetClipboardText(clipboard.str().c_str());
	}

	ImGui::Separator();

	auto& io = ImGui::GetIO();
	auto font = io.Fonts->Fonts[3];
	ImGui::PushFont(font);

	for (auto it = mLogMsgs.rbegin(); it != mLogMsgs.rend(); ++it)
	{
		ImGui::TextColored(GetColorFromSeverity(it->second), "%s", it->first.c_str());
	}
	
	ImGui::PopFont();

	ImGui::End();
	ImGui::PopStyleVar();
}

void LogPanel::PushMessage(Severity severity, const std::string& msg)
{
	mLogMsgs.emplace_back(msg, severity);
}

EditorPreferencesPanel::EditorPreferencesPanel(const std::string& name) : UIPanel(name, false)
{
	windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
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
				Engine::GetEngineInstance().ChangeEditorTheme(settings->theme);
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
				if (ImGui::Button("Select a file"))
					settings->tempFilepath = FileDialogs::OpenFile("");
				ImGui::SameLine();
				if (ImGui::Button("Reset"))
					settings->tempFilepath = "";
			}
		}

		ImGui::EndPopup();
	}
}

ImVec4 GetColorFromSeverity(Severity severity)
{
	switch (severity)
	{
	case Trace:
		return ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	case Info: 
		return ImVec4{ 0.258f, 0.815f, 1.0f, 1.0f };
	case Success:
		return ImVec4{ 0.258f, 1.0f, 0.258f, 1.0f };
	case Warn:
		return ImVec4{ 1.0f, 0.815f, 0.258f, 1.0f };
	case Error:
		return ImVec4{ 1.0f, 0.258f, 0.258f, 1.0f };
	case Fatal:
		return ImVec4{ 0.901f, 0.0f, 0.0f, 1.0f };
	}
}

void UpdateChangesPanel::DrawUI()
{
	if (mActive && !ImGui::IsPopupOpen("Save changes?"))
		ImGui::OpenPopup("Save changes?");

	if (ImGui::BeginPopupModal("Save changes?", &mActive, windowFlags))
	{
		ImGui::SetWindowSize({ 650, 95 });
		
		ImGui::TextWrapped("It looks like you haven't saved your file. Would you like to update changes before opening a new file?");
		ImGui::Separator();
		if (ImGui::Button("Yes"))
		{
			auto& engine = Engine::GetEngineInstance();
			auto sePanel = reinterpret_cast<ShaderEditorPanel*>(engine.GetUIFrames()["Shader Editor"]);

			auto fragSource = sePanel->GetFragSource();
			if (sePanel->GetFilename() == "untitled.frag")
			{
				auto result = FileDialogs::SaveFileAs("");
				sePanel->SetFilename(result == "" ? "untitled.frag" : result);
				if (result != "")
					Engine::GetEngineInstance().SaveFile(fragSource, sePanel->GetFilename());
			}
			else
				engine.SaveFile(sePanel->GetFragSource());

			switch (mAction)
			{
			case 0:
				engine.NewFile();
				break;
			case 1:
				engine.OpenFile(FileDialogs::OpenFile(""));
				break;
			case 2:
				engine.CloseFile();
				break;
			}

			mActive = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			auto& engine = Engine::GetEngineInstance();
			
			switch (mAction)
			{
			case 0:
				engine.NewFile();
				break;
			case 1:
				engine.OpenFile(FileDialogs::OpenFile(""));
				break;
			case 2:
				engine.CloseFile();
				break;
			}

			mActive = false;
		}

		ImGui::EndPopup();
	}
}

void RestoreFilePanel::DrawUI()
{
	auto settings = Engine::GetEngineInstance().GetSettings();

	if (mActive && !ImGui::IsPopupOpen("Restore File"))
		ImGui::OpenPopup("Restore File");

	if (ImGui::BeginPopupModal("Restore File", &mActive, windowFlags))
	{
		ImGui::SetWindowSize({ 650, 95 });

		ImGui::TextWrapped("The last time you closed the software, you didn't save your last active file. Would you like to restore it?");
		ImGui::Separator();
		if (ImGui::Button("Yes"))
		{
			Engine::GetEngineInstance().RestoreFile();
			std::filesystem::remove(settings->tempFilepath == "" ? "tempFile.tmp" : settings->tempFilepath);
			mActive = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("No"))
		{
			std::filesystem::remove(settings->tempFilepath == "" ? "tempFile.tmp" : settings->tempFilepath);
			mActive = false;
		}

		ImGui::EndPopup();
	}
}
