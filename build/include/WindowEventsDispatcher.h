#pragma once

#include <Events/Event.h>
#include <Events/KeyEvents.h>
#include <Events/MouseEvents.h>
#include <Events/WindowEvents.h>

#include <Window.h>

bool OnWindowClosed(WindowClosed& e);
bool OnKeyPressed(KeyPressed& e);
void WindowEventProcedure(Event& e);

