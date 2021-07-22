#pragma once

#include <FrameBuffer.h>
#include <Renderer.h>

#include "Events/Event.h"

class UIPanel
{
public:
	virtual void DrawUI() = 0;
	virtual void OnEvent(Event& e) = 0;

	std::string GetName() const { return mName; }

	bool IsActive() const { return mActive; }
	void Activate(bool state) { mActive = state; }

protected:
	UIPanel(const std::string& name, bool active = true) : mName(name), mActive(active) 
	{ windowFlags = ImGuiWindowFlags_NoCollapse; }

	std::string mName;
	bool mActive;

	ImGuiWindowFlags windowFlags;
};

class MenuBarPanel : public UIPanel
{
public:
	MenuBarPanel(const std::string& name) : UIPanel(name) {}

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e);
};

class ViewportPanel : public UIPanel
{
public:
	ViewportPanel(const std::string& name) : UIPanel(name) {}

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) { mFrameBuffer = fb; }
	glm::vec2 GetMousePos() const { return mMousePos; }

private:
	glm::vec2 mViewportSize = { 0, 0 };
	glm::vec2 mMousePos = { 0,  0 };
	std::shared_ptr<FrameBuffer> mFrameBuffer;
};

class ShaderEditorPanel : public UIPanel
{
public:
	ShaderEditorPanel(const std::string& name) : UIPanel(name) {}
	
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	std::string GetFragSource() { return mFragmentSrc; }

private:
	std::string mFragmentSrc;
};

class DemoPanel : public UIPanel
{
public:
	DemoPanel(const std::string& name) : UIPanel(name) { }
	
	virtual void DrawUI() override { ImGui::ShowDemoWindow(); }
	virtual void OnEvent(Event& e) {}
};

class LogPanel : public UIPanel
{
public: 
	LogPanel(const std::string& name) : UIPanel(name), mLogMsgs({}) 
	{ windowFlags |= ImGuiWindowFlags_AlwaysAutoResize; }

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void PushMessage(const std::string& msg);

private:
	std::vector<std::string> mLogMsgs;
};

class EditorPreferencesPanel : public UIPanel
{
public:
	EditorPreferencesPanel(const std::string& name);
	
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}
};