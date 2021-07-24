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
	glm::vec2 GetViewportSize() const { return mViewportSize; }

private:
	glm::vec2 mViewportSize = { 0, 0 };
	glm::vec2 mMousePos = { 0,  0 };
	std::shared_ptr<FrameBuffer> mFrameBuffer;
};

class ShaderEditorPanel : public UIPanel
{
public:
	ShaderEditorPanel(const std::string& name) : UIPanel(name), mSaved(true), mFilename("") {}
	
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void Save(bool save = true) { mSaved = save; }
	bool IsSaved() const { return mSaved; }

	void SetFragSource(std::string src) { mFragmentSrc = src; }
	std::string GetFragSource() const { return mFragmentSrc; }

	void SetFilename(std::string filename) { mFilename = filename; }
	std::string GetFilename() const { return mFilename; }

private:
	bool mSaved;

	std::string mFragmentSrc;
	std::string mFilename;
};


class DemoPanel : public UIPanel
{
public:
	DemoPanel(const std::string& name) : UIPanel(name) { }
	
	virtual void DrawUI() override { ImGui::ShowDemoWindow(); }
	virtual void OnEvent(Event& e) {}
};

enum Severity
{
	Trace = 0,
	Info = 1,
	Success = 2,
	Warn = 3,
	Error = 4,
	Fatal = 5
};

static ImVec4 GetColorFromSeverity(Severity severity);

class LogPanel : public UIPanel
{
public: 
	LogPanel(const std::string& name) : UIPanel(name), mLogMsgs({}) 
	{ windowFlags |= ImGuiWindowFlags_AlwaysAutoResize; }

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void PushMessage(Severity severity, const std::string& msg);

private:
	std::vector<std::pair<std::string, Severity>> mLogMsgs;
};

class EditorPreferencesPanel : public UIPanel
{
public:
	EditorPreferencesPanel(const std::string& name);
	
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}
};

class UpdateChangesPanel : public UIPanel
{
public:
	UpdateChangesPanel(const std::string& name) : UIPanel(name, false)
	{
		windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	}

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}
	
	void Activate(bool state, int action) { mActive = state; mAction = action; }

private:
	int mAction = 0;
};

class RestoreFilePanel : public UIPanel
{
public:
	RestoreFilePanel(const std::string& name) : UIPanel(name, true)
	{
		windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	}

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void Activate(bool state, int action) { mActive = state; mAction = action; }

private:
	int mAction = 0;
};

