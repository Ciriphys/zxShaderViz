#pragma once

#include "zxpch.h"

enum class EventType
{
	KeyPressed,
	KeyReleased,

	MouseMoved,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseWheelScrolled, 

	WindowResized,
	WindowMoved,
	WindowClosed,

	FilesDropped
};

class Event
{
protected:
	bool mHandled = false;

	friend class EventDispatcher;

public:
	virtual ~Event() {}

	virtual EventType	GetEventType()		const = 0;
	virtual const char* GetEventName()		const = 0;
	virtual std::string GetEventNameStr()   const = 0;
	virtual std::string GetEventInfo()		const = 0;

	virtual bool IsHandled() const = 0;

	inline operator bool() const { return IsHandled(); }
};

class EventDispatcher
{
private:
	template <typename T>
	using EventProcedure = std::function<bool(T&)>;

	Event& mEvent;
public:
	EventDispatcher(Event& e) : mEvent(e) {}

	template <typename T>
	bool Emit(EventProcedure<T> function)
	{
		if (mEvent.GetEventType() == T::GetEventStaticType())
		{
			mEvent.mHandled = function(*(T*)&mEvent);
			return true;
		}

		return false;
	}
};

#define BIND_FUNCTION(...) std::bind(__VA_ARGS__, std::placeholders::_1)