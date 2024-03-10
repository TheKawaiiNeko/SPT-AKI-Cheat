#pragma once
#include <Windows.h>

struct Globals
{
	bool Run = false;
	int MenuKey = VK_INSERT;

	RECT GameSize;
	POINT GamePoint;

	// Visual
	bool ESP = true;
	bool ESP_Line = true;
	bool ESP_Distance = true;
};

extern struct Globals g;