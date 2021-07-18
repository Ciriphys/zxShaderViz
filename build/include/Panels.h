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

	virtual void DrawUI() override;
	virtual void OnEvent(Event& e) {}

private:
	glm::vec2 mViewportSize = { 0, 0 };
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