//©2021 JDSherbert. All Rights Reserved.

#pragma once

#include <string>
#include <d3d11.h>
#include "../../Imgui/imgui.h"

class AboutWindow
{
public:
	void Init();
	void Render();
	void Shutdown();
	bool renderAbout = false;

private:
	ID3D11ShaderResourceView* _SherbertBanner = nullptr;
	std::string _VersionText = "";
};

AboutWindow& AboutClass();