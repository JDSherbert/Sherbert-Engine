#pragma once
#include "Bindable.h"
#include "BufferResource.h"
#include <optional>

class Graphics;
class Surface;

namespace Bind
{
	class DepthStencil;

	class RenderTarget : public Bindable, public BufferResource
	{
	public:
		void BindAsBuffer( Graphics& gfx ) noxnd override;
		void BindAsBuffer( Graphics& gfx,BufferResource* depthStencil ) noxnd override;
		void BindAsBuffer( Graphics& gfx,DepthStencil* depthStencil ) noxnd;
		void Clear( Graphics& gfx ) noxnd override;
		void Clear( Graphics& gfx,const std::array<float,4>& color ) noxnd;
		UINT GetWidth() const noexcept;
		UINT GetHeight() const noexcept;
	private:
		void BindAsBuffer( Graphics& gfx,ID3D11DepthStencilView* pDepthStencilView ) noxnd;
	protected:
		RenderTarget( Graphics& gfx,ID3D11Texture2D* pTexture,std::optional<UINT> face );
		RenderTarget( Graphics& gfx,UINT width,UINT height );
		UINT width;
		UINT height;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTargetView;
	};

	class ShaderInputRenderTarget : public RenderTarget
	{
	public:
		ShaderInputRenderTarget( Graphics& gfx,UINT width,UINT height,UINT slot );
		void Bind( Graphics& gfx ) noxnd override;
		Surface ToSurface( Graphics& gfx ) const;
	private:
		UINT slot;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;
	};

	class OutputOnlyRenderTarget : public RenderTarget
	{
	public:
		void Bind( Graphics& gfx ) noxnd override;
		OutputOnlyRenderTarget( Graphics& gfx,ID3D11Texture2D* pTexture,std::optional<UINT> face = {} );
	};
}