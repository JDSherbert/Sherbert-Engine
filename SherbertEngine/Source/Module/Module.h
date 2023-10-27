//©2021 JDSherbert. All Rights Reserved.

#pragma once

#include <Windows.h>
#include "../Imgui/imgui.h"

typedef void (*MYFUNC)(ImGuiContext*);

class Module
{
public:
	bool Init();

public:
	MYFUNC myFunc = nullptr;
};

Module& ModuleClass();