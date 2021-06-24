#pragma once
#include "SherbertEngine_WinHandler.h"
#include "SherbertEngine_Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>


class Window
{
public:
	//Exception Handlers
	class Exception : public SherbertEngine_Exception
	{
		using SherbertEngine_Exception::SherbertEngine_Exception;
	public:
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
	};

	//HResult Exceptions
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hresult;
	};

	//Graphics exceptions
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	//Singleton instance manages registration/cleanup of window class
	class WindowClass
	{

	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass& ) = delete;
		WindowClass& operator=( const WindowClass& ) = delete;
		static constexpr const char* wndClassName = "SherbertEngine [DirectX] Render Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	//Window instance params
	Window( int width,int height,const char* name );
	~Window();
	Window( const Window& ) = delete;
	Window& operator=( const Window& ) = delete;
	void SetTitle( const std::string& title );
	void EnableCursor() noexcept;
	void DisableCursor() noexcept;
	bool CursorEnabled() const noexcept;
	static std::optional<int> ProcessMessages() noexcept;

	//Graphics Object
	Graphics& Gfx();

private:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowCursor() noexcept;
	void HideCursor() noexcept;
	void EnableImGuiMouse() noexcept;
	void DisableImGuiMouse() noexcept;
	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;

public:
	//Input devices
	Keyboard keyboard;
	Mouse mouse;

private:
	//Params
	bool cursorEnabled = true;
	int width;
	int height;

	HWND hWnd;
	std::unique_ptr<Graphics> EngineGfxptr;
	std::vector<BYTE> rawBuffer;
	std::string commandLine;
};
