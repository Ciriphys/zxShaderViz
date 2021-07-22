#pragma once

#include "Event.h"

class WindowResized : public Event 
{
public:
	WindowResized(unsigned int width, unsigned int height) : mWidth(width), mHeight(height) {}

	virtual EventType	GetEventType()		const override { return EventType::WindowResized; }
	virtual const char* GetEventName()		const override { return "WindowResized"; }
	virtual std::string GetEventNameStr()   const override { return "WindowResized"; }
	virtual std::string GetEventInfo()		const override 
	{ 
		std::stringstream ss;
		ss << "WindowResized: [x(" << mWidth << ") | y(" << mHeight << ")]";
		return ss.str(); 
	}

	unsigned int GetWidth()  const { return mWidth;  }
	unsigned int GetHeigth() const { return mHeight; }

	static EventType GetEventStaticType() { return EventType::WindowResized; }

	virtual bool IsHandled() const override { return mHandled; }

private:
	unsigned int mWidth;
	unsigned int mHeight;
};

class WindowMoved : public Event
{
public:
	WindowMoved(unsigned int posx, unsigned int posy) : mPosx(posx), mPosy(posy) {}

	virtual EventType	GetEventType()		const override { return EventType::WindowMoved; }
	virtual const char* GetEventName()		const override { return "WindowMoved"; }
	virtual std::string GetEventNameStr()   const override { return "WindowMoved"; }
	virtual std::string GetEventInfo()		const override
	{
		std::stringstream ss;
		ss << "WindowMoved: [x(" << mPosx << ") | y(" << mPosy << ")]";
		return ss.str();
	}

	unsigned int GetPosX() const { return mPosx; }
	unsigned int GetPosY() const { return mPosy; }

	static EventType GetEventStaticType() { return EventType::WindowMoved; }

	virtual bool IsHandled() const override { return mHandled; }

private:
	unsigned int mPosx;
	unsigned int mPosy;
};

class WindowClosed : public Event 
{ 
public:
	virtual EventType	GetEventType()		const override { return EventType::WindowClosed; }
	virtual const char* GetEventName()		const override { return "WindowClosed"; }
	virtual std::string GetEventNameStr()   const override { return "WindowClosed"; }
	virtual std::string GetEventInfo()		const override { return "WindowClosed [No info]"; }

	static EventType GetEventStaticType() { return EventType::WindowClosed; }

	virtual bool IsHandled() const override { return mHandled; }
};

class AppTick : public Event 
{ 
public:
	virtual EventType	GetEventType()		const override { return EventType::AppTick; }
	virtual const char* GetEventName()		const override { return "AppTick"; }
	virtual std::string GetEventNameStr()   const override { return "AppTick"; }
	virtual std::string GetEventInfo()		const override { return "AppTick [No info]"; }

	static EventType GetEventStaticType() { return EventType::AppTick; }

	virtual bool IsHandled() const override { return mHandled; }
};

class AppRender : public Event 
{
public:
	virtual EventType	GetEventType()		const override { return EventType::AppRender; }
	virtual const char* GetEventName()		const override { return "AppRender"; }
	virtual std::string GetEventNameStr()   const override { return "AppRender"; }
	virtual std::string GetEventInfo()		const override { return "AppRender [No info]"; }

	static EventType GetEventStaticType() { return EventType::AppRender; }

	virtual bool IsHandled() const override { return mHandled; }
};

class FilesDropped : public Event
{
public:
	FilesDropped(int number, const char* files[])
	{
		for (int i = 0; i < number; i++)
			mFiles.push_back(files[i]);
	}

	virtual EventType	GetEventType()		const override { return EventType::FilesDropped; }
	virtual const char* GetEventName()		const override { return "FilesDropped"; }
	virtual std::string GetEventNameStr()   const override { return "FilesDropped"; }
	virtual std::string GetEventInfo()		const override 
	{ 
		std::stringstream ss;
		ss << "FilesDropped: [number(" << mFiles.size() << ")]\n";
		for (auto& file : mFiles)
			ss << file << std::endl;
		return ss.str();
	}

	static EventType GetEventStaticType() { return EventType::FilesDropped; }

	virtual bool IsHandled() const override { return mHandled; }

	std::vector<std::string> GetFiles() const { return mFiles; }

private:
	std::vector<std::string> mFiles;
};
