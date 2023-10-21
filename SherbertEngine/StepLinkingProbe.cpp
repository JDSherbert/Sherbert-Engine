// ©2021 JDSherbert. All rights reserved.

#include "StepLinkingProbe.h"
#include "RenderGraph.h"
#include "Step.h"

void StepLinkingProbe::OnSetStep()
{
	pStep->Link( rg );
}