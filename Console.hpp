#pragma once
#include <iostream>
#include <windows.h>
#pragma warning(disable: 26812)

namespace console
{
	enum Color
	{
		Green = FOREGROUND_GREEN,
		Red = FOREGROUND_RED,
		Blue = FOREGROUND_BLUE,
		Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
		Yellow = FOREGROUND_GREEN | FOREGROUND_RED,
		Magenta = FOREGROUND_BLUE | FOREGROUND_RED,
		White = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED
	};

	inline void SetStdOutColor(Color c)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(out, c | FOREGROUND_INTENSITY);
	}
}

#pragma warning(default: 26812)