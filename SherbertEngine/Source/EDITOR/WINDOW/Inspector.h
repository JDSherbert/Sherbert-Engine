//©2021 JDSherbert. All Rights Reserved.

#pragma once

#include <entt/entt.hpp>
#include "../../Imgui/imgui.h"

class InspectorWindow
{
public:
	void Render();
	bool renderInspector = true;

private:
	void RenderAdd();

private:
	template <typename T>
	void RenderComponent(entt::entity entity);
};

InspectorWindow& InspectorClass();