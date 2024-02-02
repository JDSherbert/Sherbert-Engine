//©2021 JDSherbert. All Rights Reserved.

#include "../Imgui/imgui.h"

// ----- Defs ----- //
#define IMGUI_CONFIG_PATH "../Config/imgui.ini" // Config File must Exist Before saving otherwise it will not be created


// ----- Func ----- //
bool CreateImguiConfigFolder(); // Create the Config Folder
ImGuiIO& SetIniFilename(ImGuiIO& io); // Set the Config File Name See Imgui.h " const char* IniFilename;   " 