#include "sppch.h"

#include "Utils/Input.h"
#include "Engine.h"
#include "Window.h"

#include <GLFW/glfw3.h>

namespace Input
{
    bool GetKeyPress(Key key)
    {
        auto window = Engine::GetEngineInstance().GetWindow()->GetNativeWindow();
        bool state = glfwGetKey(window, (int)key);
        return state == GLFW_PRESS;
    }

    bool GetKeyRelease(Key key)
    {
        auto window = Engine::GetEngineInstance().GetWindow()->GetNativeWindow();
        bool state = glfwGetKey(window, (int)key);
        return state == GLFW_RELEASE;
    }

    bool GetKey(Key key)
    {
        auto window = Engine::GetEngineInstance().GetWindow()->GetNativeWindow();
        bool state = glfwGetKey(window, (int)key);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GetMouseButtonPress(Key button)
    {
        auto window = Engine::GetEngineInstance().GetWindow()->GetNativeWindow();
        bool state = glfwGetMouseButton(window, (int)button);
        return state == GLFW_PRESS;
    }

    bool GetMouseButtonRelease(Key button)
    {
        auto window = Engine::GetEngineInstance().GetWindow()->GetNativeWindow();
        bool state = glfwGetMouseButton(window, (int)button);
        return state == GLFW_RELEASE;
    }
}