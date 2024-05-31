#pragma once

#include <windows.h>

namespace win {
	inline bool set_consoleCP_to_UTF8()
	{
		return SetConsoleOutputCP(56001) && SetConsoleCP(56001);
	}
}