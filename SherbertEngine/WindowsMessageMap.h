// ©2021 JDSherbert. All rights reserved.

#pragma once

#include <unordered_map>
#include <string>

#include "SherbertWin.h"

class WindowsMessageMap
{

public:
	WindowsMessageMap() noexcept;
	std::string operator()( DWORD msg,LPARAM lp,WPARAM wp ) const noexcept;

private:
	std::unordered_map<DWORD,std::string> map;
};