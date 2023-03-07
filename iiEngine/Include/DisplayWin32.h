#pragma once

#include "Game.h"

class DisplayWin32 {
protected:
	LPCWSTR applicationName;
	RECT windowRect;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	int clientHeight;
	int clientWidth;
	int screenWidth;
	int screenHeight;

public:
	DisplayWin32(LPCWSTR& applicationName, int screenWidth, int screenHeight, WNDPROC wndProc);

	int GetScreenWidth();
	int GetScreenHeight();
	HWND& GetHWnd();
};
