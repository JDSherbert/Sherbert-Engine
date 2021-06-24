#pragma once
#include "FullscreenPass.h"
#include "ConstantBuffersEx.h"

class Graphics;
namespace Bind
{
	class PixelShader;
	class RenderTarget;
}

namespace n_SherbRendergraphpass
{
	class VerticalBlurPass : public FullscreenPass
	{
	public:
		VerticalBlurPass( std::string name,Graphics& gfx );
		void Execute( Graphics& gfx ) const noxnd override;
	private:
		std::shared_ptr<Bind::CachingPixelConstantBufferEx> direction;
	};
}