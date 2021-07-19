#pragma once

#include "Keycode.h"

namespace Input
{
	bool GetKeyPress				(Key key);
	bool GetKeyRelease				(Key key);
	bool GetKey						(Key key);
	
	bool GetMouseButtonPress		(Key button);
	bool GetMouseButtonRelease		(Key button);
}