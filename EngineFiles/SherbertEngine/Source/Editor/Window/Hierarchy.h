//©2021 JDSherbert. All Rights Reserved.

#pragma once

#include <entt/entt.hpp>
#include "../../Imgui/imgui.h"

class HierarchyWindow
{
public:
	void Render();
	bool renderHierarchy = true;
private:
	void RenderTree(entt::entity entity);
};

HierarchyWindow& HierarchyClass();