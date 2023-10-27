#pragma once

#include <Windows.h>
#include <string>

class SplashScreen
{
public:
	bool ShowSplashScreen();
	void HideSplashScreen();

private:
	HWND hwnd = NULL;
	std::wstring name = L"SherbertEngineSplashScreen";
	UINT width = 600;
	UINT height = 200;
	std::wstring path = L"Resources\\Images\\SplashScreen.bmp";
};

SplashScreen& SplashScreenClass();