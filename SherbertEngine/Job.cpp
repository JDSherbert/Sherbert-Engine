// �2021 JDSherbert. All rights reserved.

#include "Job.h"
#include "Step.h"
#include "Drawable.h"


namespace Rgph
{
	Job::Job( const Step* pStep,const Drawable* pDrawable )
		:
		pDrawable{ pDrawable },
		pStep{ pStep }
	{}

	void Job::Execute( Graphics& gfx ) const noxnd
	{
		pDrawable->Bind( gfx );
		pStep->Bind( gfx );
		gfx.DrawIndexed( pDrawable->GetIndexCount() );
	}
}