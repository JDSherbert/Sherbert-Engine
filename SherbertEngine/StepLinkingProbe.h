// ©2021 JDSherbert. All rights reserved.

#pragma once

#include "TechniqueProbe.h"

namespace Rgph
{
	class RenderGraph;
}

class StepLinkingProbe : public TechniqueProbe
{
protected:
	void OnSetStep() override;
private:
	Rgph::RenderGraph& rg;
};