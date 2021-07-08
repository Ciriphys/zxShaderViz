#pragma once

#include "sppch.h"

enum class EventType
{
	KeyPressed,
	KeyReleased,

	MouseMoved,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseWheelScrolled, 

	WindowResized,
	WindowClosed,

	AppTick,
	AppRender,
};

class Event
{
protected:
	bool m_Handled = false;

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

	Event& m_Event;
public:
	EventDispatcher(Event& e) : m_Event(e) {}

	template <typename T>
	bool Emit(EventProcedure<T> function)
	{
		if (m_Event.GetEventType() == T::GetEventStaticType())
		{
			m_Event.m_Handled = function(*(T*)&m_Event);
			return true;
		}

		return false;
	}
};

#define BIND_FUNCTION(...) std::bind(__VA_ARGS__, std::placeholders::_1)