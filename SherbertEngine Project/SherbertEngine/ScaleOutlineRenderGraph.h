#pragma once
#include "RenderGraph.h"

class Graphics;

namespace n_SherbRendergraphpass
{
	class ScaleOutlineRenderGraph : public RenderGraph
	{
	public:
		ScaleOutlineRenderGraph( Graphics& gfx );
	};
}