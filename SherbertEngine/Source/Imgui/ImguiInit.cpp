//©2021 JDSherbert. All Rights Reserved.

#include "ImguiInit.h"

#include "../Core/Utils.h"
#include <iostream>

bool CreateImguiConfigFolder()
{
	// If -> " Config " folder doesn't exist, create it in the SolutionDir.
	if (!Utils::CreateDirectory("../Config"))
	{
		std::cerr << "Failed to create directory." << std::endl;
		return 1;
	}
	return 0;
}

ImGuiIO& SetIniFilename(ImGuiIO& io)
{
	io.IniFilename = IMGUI_CONFIG_PATH;
	std::cerr << "Succesfully Created Ini FileName" << std::endl;
	return io;
}




