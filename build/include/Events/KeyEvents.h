#pragma once

#include "Event.h"

class KeyEvent : public Event
{
protected:
	int mKeycode;
	KeyEvent(int keycode) : mKeycode(keycode) {}
public:
	inline int GetKeyCode() const { return mKeycode; }
};

class KeyPressed : public KeyEvent
{
public:
	KeyPressed(int keycode, int count) : KeyEvent(keycode), mCount(count) {}

	inline int GetCount() const { return mCount; }

	virtual EventType	GetEventType()		const override { return EventType::KeyPressed; }
	virtual const char* GetEventName()		const override { return "KeyPressed"; }
	virtual std::string GetEventNameStr()	const override { return "KeyPressed"; }
	virtual bool		IsHandled()			const override { return mHandled; }

	virtual std::string GetEventInfo() const override 
	{
		std::stringstream ss;
		ss << "KeyPressed: [Keycode(" << mKeycode << ") | Repetitions(" << mCount << ")]";
		return ss.str();
	}

	static EventType GetEventStaticType() { return EventType::KeyPressed; }

private:
	int mCount;
};

class KeyReleased : public KeyEvent
{
public:
	KeyReleased(int keycode) : KeyEvent(keycode) {}

	virtual EventType   GetEventType()		const override { return EventType::KeyReleased; }
	virtual const char* GetEventName()		const override { return "KeyReleased"; }
	virtual std::string GetEventNameStr()	const override { return "KeyReleased"; }
	virtual bool		IsHandled()			const override { return mHandled; }

	virtual std::string GetEventInfo() const override
	{
		std::stringstream ss;
		ss << "KeyReleased: [Keycode(" << mKeycode << ")]";
		return ss.str();
	}

	static EventType GetEventStaticType() { return EventType::KeyReleased; }
};
