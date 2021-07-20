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

	template<typename T>
	const T* as() const { return reinterpret_cast<T*>(this); }

protected:
	UIPanel(const std::string& name) : mName(name) {}
	std::string mName;
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
	void SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) { mFrameBuffer = fb; }

	glm::vec2 GetMousePos() const { return mMousePos; }

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

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

	std::vector<std::string> GetSources() { return { mVertexSrc, mFragmentSrc }; }

private:
	std::string mVertexSrc;
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
	LogPanel(const std::string& name) : UIPanel(name), mLogMsgs({}) { }
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

	void PushMessage(const std::string& msg);

private:
	std::vector<std::string> mLogMsgs;
};

class DirectoryExplorerPanel : public UIPanel
{
public:
	DirectoryExplorerPanel(const std::string& name) : UIPanel(name), mCurrentDirectory(""), mFiles({}) { }
	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

private:
	std::string Trim(const std::string& path);
	
	std::filesystem::path mCurrentDirectory;
	std::vector<std::filesystem::path> mFiles;
};