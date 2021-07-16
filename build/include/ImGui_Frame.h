#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <FrameBuffer.h>
#include <Renderer.h>

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
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
        ImGui::Begin("MenuBar", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspace_id = ImGui::GetID("MenuBarDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New",  "Ctrl+N", nullptr);
                ImGui::MenuItem("Open", "Ctrl+O", nullptr);
                ImGui::MenuItem("Save", "Ctrl+S", nullptr);

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
};

class ViewportFrame : public UIFrame
{
public: 
    void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) { mFrameBuffer = fb; }

    virtual void DrawUI() override;

private:
    glm::vec2 mViewportSize = {0, 0};
    std::shared_ptr<FrameBuffer> mFrameBuffer;
};