#pragma once
#include "TechniqueProbe.h"

namespace n_SherbRendergraphpass
{
	class RenderGraph;
}

class StepLinkingProbe : public TechniqueProbe
{
protected:
	void OnSetStep() override;
private:
	n_SherbRendergraphpass::RenderGraph& RenderGraphSteps;
};