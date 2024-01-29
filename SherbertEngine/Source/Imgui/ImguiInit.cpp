//©2021 JDSherbert. All Rights Reserved.

#include "ImguiInit.h"

#include "../Core/Utils.h"

ImGuiIO& CreateImguiConfig()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Possible change To change the location of the imgui.ini file generated and loaded by ImGui.
	// ../ Go back in directory to (SolutionDir)
	// Make the specified folder if it doesn't exist. Or Place to the folder you want to save the imgui.ini file.
	// define name for imgui.ini
	io.IniFilename = IMGUI_CONFIG_PATH;

	return io;
}


