#pragma once

#include <GLFW/glfw3.h>

class TimeStep
{
public:
	double GetDeltaTime() const { return deltaTime; }
	float GetDeltaTimef() const { return static_cast<float>(deltaTime); }

	double GetExecutionTime() const { return glfwGetTime(); }
	float GetExecutionTimef() const { return static_cast<float>(glfwGetTime()); }

	void Update()
	{
		double currentFrameTime = glfwGetTime();
		deltaTime = lastFrameTime - currentFrameTime;
		lastFrameTime = currentFrameTime;
	}

private:
	double lastFrameTime = 0.0;
	double deltaTime = 0.0;
};