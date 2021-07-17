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

	std::string GetName() const { return mName; }

protected:
	UIFrame(const std::string& name) : mName(name) {}
	std::string mName;
};

class MenuBarFrame : public UIFrame
{
public:
	MenuBarFrame(const std::string& name) : UIFrame(name) {}
	virtual void DrawUI() override;

};

class ViewportFrame : public UIFrame
{
public: 
	ViewportFrame(const std::string& name) : UIFrame(name) {}
    void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) { mFrameBuffer = fb; }

    virtual void DrawUI() override;

private:
    glm::vec2 mViewportSize = {0, 0};
    std::shared_ptr<FrameBuffer> mFrameBuffer;
};

class ShaderEditorFrame : public UIFrame
{
public:
	ShaderEditorFrame(const std::string& name) : UIFrame(name) {}
    virtual void DrawUI() override;
	
	std::vector<std::string> GetSources() { return {mVertexSrc, mFragmentSrc}; }

private:
    std::string mVertexSrc;
    std::string mFragmentSrc;
};