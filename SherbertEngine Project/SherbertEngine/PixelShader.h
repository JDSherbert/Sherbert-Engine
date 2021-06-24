#pragma once
#include "Bindable.h"

//Bind rasterizer to buffer via UID
namespace Bind
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader( Graphics& gfx,const std::string& path );
		void Bind( Graphics& gfx ) noxnd override;
		static std::shared_ptr<PixelShader> Resolve( Graphics& gfx,const std::string& path );
		static std::string GenerateUID( const std::string& path );
		std::string GetUID() const noexcept override;
	protected:
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	};
}