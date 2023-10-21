// ©2021 JDSherbert. All rights reserved.

#pragma once

#define SHERBERT_WINDOW_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define SHERBERT_WINDOW_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define SHERBERT_WINDOW_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )
