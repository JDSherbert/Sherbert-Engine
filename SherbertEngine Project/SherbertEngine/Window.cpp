#include "Window.h"
#include <sstream>
#include "resource.h"
#include "WindowsThrowMacros.h"
#include "imgui/imgui_impl_win32.h"


//Window Class Stuff, created once at runtime
Window::WindowClass Window::WindowClass::wndClass;

//Create first and only instance
Window::WindowClass::WindowClass() noexcept
	:
	hInst( GetModuleHandle( nullptr ) )
{
	//Window Class set to immediate first instance
	WNDCLASSEX WindowClassEX = { 0 };
	WindowClassEX.cbSize = sizeof( WindowClassEX );
	WindowClassEX.style = CS_OWNDC;
	WindowClassEX.lpfnWndProc = HandleMsgSetup;
	WindowClassEX.cbClsExtra = 0;
	WindowClassEX.cbWndExtra = 0;
	WindowClassEX.hInstance = GetInstance();
	WindowClassEX.hIcon = static_cast<HICON>(LoadImage( 
		GetInstance(),MAKEINTRESOURCE( IDI_ICON1 ),
		IMAGE_ICON,32,32,0	//Application Icon
	));
	WindowClassEX.hCursor = nullptr;
	WindowClassEX.hbrBackground = nullptr;
	WindowClassEX.lpszMenuName = nullptr;
	WindowClassEX.lpszClassName = GetName();
	WindowClassEX.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(),MAKEINTRESOURCE( IDI_ICON1 ),
		IMAGE_ICON,16,16,0
	));
	RegisterClassEx( &WindowClassEX );
}

//Kill instance
Window::WindowClass::~WindowClass()
{
	UnregisterClass( wndClassName,GetInstance() );
}

//Pointer to window name
const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

//Hresult instance
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

//Window Stuff
Window::Window( int width,int height,const char* name )
	:
	width( width ),
	height( height )
{
	//Calculate window size based on desired client region size
	RECT windowratio;
	windowratio.left = 100;
	windowratio.right = width + windowratio.left;
	windowratio.top = 100;
	windowratio.bottom = height + windowratio.top;
	if( AdjustWindowRect( &windowratio,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE ) == 0 )
	{
		throw CHWND_LAST_EXCEPT();
	}

	//Create window and get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(),name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,windowratio.right - windowratio.left,windowratio.bottom - windowratio.top,
		nullptr,nullptr,WindowClass::GetInstance(),this
	);

	//Check for error
	if( hWnd == nullptr )
	{
		throw CHWND_LAST_EXCEPT();
	}

	//Newly created windows start off as hidden
	ShowWindow( hWnd,SW_SHOWDEFAULT );

	//IMPORTANT - Initialize ImGui Win32 Impl
	ImGui_ImplWin32_Init( hWnd );

	//Create graphics object - AFTER UI
	EngineGfxptr = std::make_unique<Graphics>( hWnd,width,height );

	//Register mouse raw input device
	RAWINPUTDEVICE rawinputdevice;
	rawinputdevice.usUsagePage = 0x01; //Mouse page
	rawinputdevice.usUsage = 0x02; //Mouse usage
	rawinputdevice.dwFlags = 0;
	rawinputdevice.hwndTarget = nullptr;
	if( RegisterRawInputDevices( &rawinputdevice,1,sizeof( rawinputdevice ) ) == FALSE )
	{
		throw CHWND_LAST_EXCEPT();
	}
}

//Kill window
Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow( hWnd );
}

//Set the title of the window
void Window::SetTitle( const std::string& title )
{
	if( SetWindowText( hWnd,title.c_str() ) == 0 )
	{
		throw CHWND_LAST_EXCEPT();
	}
}

//Cursor On
void Window::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
}

//Cursor Off
void Window::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
}

//Get cursor is enabled
bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

//Window template
std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;

	// While queue has messages, remove and dispatch them (but do not block on empty queue)
	while( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		//Check for quit because peekmessage does not signal this via the return value
		if( msg.message == WM_QUIT )
		{
			//Return optional wrapping int (arg to PostQuitMessage is in wparam) - this signals quitting
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//Return empty optional when not quitting app
	return {};
}

//Graphics object
Graphics& Window::Gfx()
{
	if( !EngineGfxptr )
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *EngineGfxptr;
}

//Lock cursor to window bounds only
void Window::ConfineCursor() noexcept
{
	RECT rect; 
	GetClientRect( hWnd,&rect );
	MapWindowPoints( hWnd,nullptr,reinterpret_cast<POINT*>(&rect),2 );
	ClipCursor( &rect );
}

//Unlock cursor
void Window::FreeCursor() noexcept
{
	ClipCursor( nullptr );
}

void Window::HideCursor() noexcept
{
	while( ::ShowCursor( FALSE ) >= 0 );
}

void Window::ShowCursor() noexcept
{
	while( ::ShowCursor( TRUE ) < 0 );
}

void Window::EnableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	//Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if( msg == WM_NCCREATE )
	{
		//Then extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//Set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd) );
		//Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk) );
		//Push the message to window instance handler
		return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
	}
	//If message is recieved before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd,msg,wParam,lParam );
}

LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	//Retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr( hWnd,GWLP_USERDATA ));
	//Forward message to window instance handler
	return pWnd->HandleMsg( hWnd,msg,wParam,lParam );
}

LRESULT Window::HandleMsg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	if( ImGui_ImplWin32_WndProcHandler( hWnd,msg,wParam,lParam ) )
	{
		return true;
	}
	const auto& imio = ImGui::GetIO();

	switch( msg )
	{
	//DefProc should not handle this message because the destructor should destroy the window, so return instead of break
	case WM_CLOSE:
		PostQuitMessage( 69 );
		return 0;
	//Remove the keystate when window loses focus to prevent input getting "stuck"
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
	case WM_ACTIVATE:

		//Confine/Free the cursor on window to foreground/background if cursor disabled
		if( !cursorEnabled )
		{
			if( wParam & WA_ACTIVE )
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;

	/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:

	//The syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		//Method to stop this keyboard message if imgui wants to capture
		if( imio.WantCaptureKeyboard )
		{
			break;
		}
		if( !(lParam & 0x40000000) || keyboard.AutorepeatIsEnabled() ) //Autorepeat detection
		{
			keyboard.OnKeyPressed( static_cast<unsigned char>(wParam) );
		}
		break;

	case WM_KEYUP:

	case WM_SYSKEYUP:
		//Method to stop this keyboard message if imgui wants to capture
		if( imio.WantCaptureKeyboard )
		{
			break;
		}
		keyboard.OnKeyReleased( static_cast<unsigned char>(wParam) );
		break;

	case WM_CHAR:
		//Method to stop this keyboard message if imgui wants to capture
		if( imio.WantCaptureKeyboard )
		{
			break;
		}
		keyboard.OnChar( static_cast<unsigned char>(wParam) );
		break;

	/*********** END KEYBOARD MESSAGES ***********/

	/************* MOUSE MESSAGES ****************/
	//On mouse moved
	case WM_MOUSEMOVE:
	{
		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		//Cursorless exclusive mouse movement gets first dibs
		if( !cursorEnabled )
		{
			if( !mouse.IsInWindow() )
			{
				SetCapture( hWnd );
				mouse.OnMouseEnter();
				HideCursor();
			}
			break;
		}

		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}

		//IF IN CLIENT REGION -> log move, and log enter + capture mouse (if not previously in window)
		if( screencast_to_mouseptr.x >= 0 && screencast_to_mouseptr.x < width && screencast_to_mouseptr.y >= 0 && screencast_to_mouseptr.y < height )
		{
			mouse.OnMouseMove( screencast_to_mouseptr.x,screencast_to_mouseptr.y );
			if( !mouse.IsInWindow() )
			{
				SetCapture( hWnd );
				mouse.OnMouseEnter();
			}
		}
		//IF NOT IN CLIENT REGION -> log move / maintain capture if button down
		else
		{
			if( wParam & (MK_LBUTTON | MK_RBUTTON) )
			{
				mouse.OnMouseMove( screencast_to_mouseptr.x,screencast_to_mouseptr.y );
			}
			//IF button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}

	//Click L
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow( hWnd );
		if( !cursorEnabled )
		{
			ConfineCursor();
			HideCursor();
		}

		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}

		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		mouse.OnLeftPressed( screencast_to_mouseptr.x,screencast_to_mouseptr.y );
		break;
	}

	//Click R
	case WM_RBUTTONDOWN:
	{
		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}
		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		mouse.OnRightPressed( screencast_to_mouseptr.x,screencast_to_mouseptr.y );
		break;
	}

	//Release L
	case WM_LBUTTONUP:
	{
		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}
		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		mouse.OnLeftReleased( screencast_to_mouseptr.x,screencast_to_mouseptr.y );

		//Eject mouse if outside of window
		if( screencast_to_mouseptr.x < 0 || screencast_to_mouseptr.x >= width || screencast_to_mouseptr.y < 0 || screencast_to_mouseptr.y >= height )
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}

	//Release R
	case WM_RBUTTONUP:
	{
		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}
		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		mouse.OnRightReleased( screencast_to_mouseptr.x,screencast_to_mouseptr.y );

		//Eject mouse if outside of window
		if( screencast_to_mouseptr.x < 0 || screencast_to_mouseptr.x >= width || screencast_to_mouseptr.y < 0 || screencast_to_mouseptr.y >= height )
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}

	//Mousewheel click
	case WM_MOUSEWHEEL:
	{
		//Stop mouse messages if imgui wants to capture
		if( imio.WantCaptureMouse )
		{
			break;
		}
		const POINTS screencast_to_mouseptr = MAKEPOINTS( lParam );
		const int mouseDelta = GET_WHEEL_DELTA_WPARAM( wParam );
		mouse.OnWheelDelta( screencast_to_mouseptr.x,screencast_to_mouseptr.y,mouseDelta );
		break;
	}
	/************** END MOUSE MESSAGES **************/
	
	/************** RAW MOUSE MESSAGES **************/
	//On mouse input
	case WM_INPUT:
	{
		if( !mouse.RawEnabled() )
		{
			break;
		}
		UINT size;
		//First get the size of the input data
		if( GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof( RAWINPUTHEADER ) ) == -1)
		{
			//Stop msg processing if error
			break;
		}
		rawBuffer.resize( size );
		//Read in the input data from size
		if( GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof( RAWINPUTHEADER ) ) != size )
		{
			//Stop msg processing if error
			break;
		}
		//Finally process the raw input data
		auto& rawinput = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if( rawinput.header.dwType == RIM_TYPEMOUSE &&
			(rawinput.data.mouse.lLastX != 0 || rawinput.data.mouse.lLastY != 0) )
		{
			mouse.OnRawDelta( rawinput.data.mouse.lLastX,rawinput.data.mouse.lLastY );
		}
		break;
	}
	/************** END RAW MOUSE MESSAGES **************/
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}


//Window Exception Stuff
std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	char* pMsgBuf = nullptr;
	//MS Windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,hr,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
	);
	//0 string length returned indicates a failure
	if( nMsgLen == 0 )
	{
		return "Unidentified error code";
	}
	//Copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	//Free windows buffer
	LocalFree( pMsgBuf );
	return errorString;
}

//Hresult exception
Window::HrException::HrException( int line,const char* file,HRESULT hr ) noexcept
	:
	Exception( line,file ),
	hresult( hr )
{}

//Exception handling - Convert code to string outputs
const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "SherbertEngine Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hresult;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode( hresult );
}


const char* Window::NoGfxException::GetType() const noexcept
{
	return "SherbertEngine Window Exception [No Graphics]";
}