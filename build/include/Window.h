#pragma once

#include <Events/Event.h>
#include <string>

struct GLFWwindow;
using EventProcedure = std::function<void(Event&)>;

enum RefreshRate : unsigned int
{
	RefreshRate_Unlimited = 0,
	RefreshRate_60hz = 1,
	RefreshRate_30hz = 2
};

class Window
{
public:
	Window
	(
		uint32_t width = 1280,
		uint32_t height = 720,
		std::string title = "Window",
		RefreshRate rrate = RefreshRate_60hz
	) 
	{
		m_Data.m_Width			= width;
		m_Data.m_Height			= height;
		m_Data.m_Title			= title;
		m_Data.m_RefreshRate	= rrate;

		Init();
	}

	~Window() = default;

	uint32_t	 GetWidth()			const { return m_Data.m_Width;		 }
	uint32_t	 GetHeight()		const { return m_Data.m_Height;		 }
	std::string  GetTitle()			const { return m_Data.m_Title;		 }
	RefreshRate  GetRefreshRate()	const { return m_Data.m_RefreshRate; }

	bool		 IsVsync()   const { return m_Data.m_RefreshRate == RefreshRate_60hz;  }
	bool		 IsActive()	 const { return m_IsActive; }

	void SetRefreshRate(RefreshRate);
	void SetTitle(std::string);

	void SetEventCallbackProcedure(const EventProcedure&);

	void Update();
	void Close();

private:
	void Init();

	GLFWwindow* m_Window;
	
	struct WindowData
	{
		uint32_t m_Width, m_Height;
		std::string m_Title;
		RefreshRate m_RefreshRate;

		EventProcedure m_Procedure;
	};

	bool m_IsActive = false;
	WindowData m_Data;
};
