#pragma once

#include <Events/Event.h>
#include <string>

struct GLFWwindow;
using EventProcedure = std::function<void(Event&)>;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

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
		mData.mWidth			= width;
		mData.mHeight			= height;
		mData.mTitle			= title;
		mData.mRefreshRate	= rrate;

		Init();
	}

	~Window() = default;

	uint32_t	 GetWidth()			const { return mData.mWidth;		 }
	uint32_t	 GetHeight()		const { return mData.mHeight;		 }
	std::string  GetTitle()			const { return mData.mTitle;		 }
	RefreshRate  GetRefreshRate()	const { return mData.mRefreshRate; }
	GLFWwindow* GetNativeWindow()   const { return mWindow; }

	bool		 IsVsync()   const { return mData.mRefreshRate == RefreshRate_60hz;  }
	bool		 IsActive()	 const { return mIsActive; }

	void SetRefreshRate(RefreshRate);
	void SetTitle(std::string);

	void SetEventCallbackProcedure(const EventProcedure&);

	void Update();
	void Clear();
	void Close();

private:
	void Init();

	GLFWwindow* mWindow;
	
	struct WindowData
	{
		uint32_t mWidth, mHeight;
		std::string mTitle;
		RefreshRate mRefreshRate;

		EventProcedure mProcedure;
	};

	bool mIsActive = false;
	WindowData mData;
};
