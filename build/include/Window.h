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

struct WindowSettings
{
	int windowSizex;
	int windowSizey;
	int windowPosx;
	int windowPosy;

	RefreshRate refreshRate;
};

class Window
{
public:
	Window
	(
		WindowSettings settings,
		std::string title = "ZeXo Shading"
	) 
	{
		mData.mSettings = settings;
		mData.mTitle = title;

		Init(settings);
	}

	~Window() = default;

	uint32_t	 GetWidth()			const { return mData.mSettings.windowSizex; }
	uint32_t	 GetHeight()		const { return mData.mSettings.windowSizey; }
	
	uint32_t	 GetPosX()		const { return mData.mSettings.windowPosx; }
	uint32_t	 GetPosY()		const { return mData.mSettings.windowPosy; }


	uint32_t	 GetCurrPosX();
	uint32_t	 GetCurrPosY();

	GLFWwindow*  GetNativeWindow()  const { return mWindow; }
	std::string  GetTitle()			const { return mData.mTitle; }
	RefreshRate  GetRefreshRate()	const { return mData.mSettings.refreshRate; }

	bool		 IsVsync()		 const { return mData.mSettings.refreshRate == RefreshRate_60hz; }
	bool		 IsActive()		 const { return mData.mIsActive; }

	void SetRefreshRate(RefreshRate);
	void SetTitle(std::string);

	void SetEventCallbackProcedure(const EventProcedure&);

	void Update();
	void Clear();
	void Close();

private:
	void Init(WindowSettings);

	GLFWwindow* mWindow;
	
	struct WindowData
	{
		WindowSettings mSettings;

		bool mIsActive = false;
		std::string mTitle;
		EventProcedure mProcedure;
	};

	WindowData mData;
};
