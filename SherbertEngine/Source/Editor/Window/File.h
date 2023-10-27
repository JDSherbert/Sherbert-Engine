//©2021 JDSherbert. All Rights Reserved.

#pragma once

#include "../../Imgui/imgui.h"

class FileWindow
{
public:
	void Render();
	bool renderFile = true;
};

FileWindow& FileClass();