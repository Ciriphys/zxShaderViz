#pragma once

#include "Keycode.h"

namespace Input
{
	static bool GetKeyPress					(Key key);
	static bool GetKeyRelease				(Key key);
	static bool GetKey						(Key key);
	 
	static bool GetMouseButtonPress			(Key button);
	static bool GetMouseButtonRelease		(Key button);
}