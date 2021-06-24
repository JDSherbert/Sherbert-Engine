#pragma once
#include <DirectXMath.h>
#include "Frustum.h"

class Graphics;
namespace n_SherbRendergraphpass
{
	class RenderGraph;
}

class Projection
{
public:
	Projection( Graphics& gfx,float width,float height,float nearZ,float farZ );
	DirectX::XMMATRIX GetMatrix() const;
	void RenderWidgets( Graphics& gfx );
	void SetPos( DirectX::XMFLOAT3 );
	void SetRotation( DirectX::XMFLOAT3 );
	void Submit( size_t channel ) const;
	void LinkTechniques( n_SherbRendergraphpass::RenderGraph& rg );
	void Reset( Graphics& gfx );
private:
	float width;
	float height;
	float nearZ;
	float farZ;
	float homeWidth;
	float homeHeight;
	float homeNearZ;
	float homeFarZ;
	Frustum frust;
};