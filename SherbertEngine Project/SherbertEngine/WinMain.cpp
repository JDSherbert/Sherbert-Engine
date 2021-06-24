#include "SherbertEngine_Application.h"

//Main Method - Also see SherbertEngine_Application and documentation 

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow )
{
	// ------------------- DEBUG CONSOLE ------------------------
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	// ----------------------------------------------------------
	std::cout << "Starting SherbertEngine.exe..." << std::endl;

	try
	{
		//Try game loop
		return SherbertEngine_Application{ lpCmdLine }.GameLoop();
	}
	catch( const SherbertEngine_Exception& e )
	{
		//Engine is broken
		MessageBox( nullptr,e.what(),e.GetType(),MB_OK | MB_ICONEXCLAMATION );
	}
	catch( const std::exception& e )
	{
		//Windows is broken
		MessageBox( nullptr,e.what(),"Standard Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	catch( ... )
	{
		//Complete error
		MessageBox( nullptr,"No details available","Unknown Exception",MB_OK | MB_ICONEXCLAMATION );
	}
	//Everything either broke or exited the application
	return -69;
}