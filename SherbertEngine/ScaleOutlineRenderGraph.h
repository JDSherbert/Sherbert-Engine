// ©2021 JDSherbert. All rights reserved.

#pragma once

#include "RenderGraph.h"

class Graphics;

namespace Rgph
{
	class ScaleOutlineRenderGraph : public RenderGraph
	{
	public:
		ScaleOutlineRenderGraph( Graphics& gfx );
	};
}