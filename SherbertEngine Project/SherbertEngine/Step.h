#pragma once
#include <vector>
#include <memory>
#include "Bindable.h"
#include "Graphics.h"

class TechniqueProbe;
class Drawable;

namespace n_SherbRendergraphpass
{
	class RenderQueuePass;
	class RenderGraph;
}

class Step
{
public:
	Step( std::string targetPassName );
	Step( Step&& ) = default;
	Step( const Step& src ) noexcept;
	Step& operator=( const Step& ) = delete;
	Step& operator=( Step&& ) = delete;
	void AddBindable( std::shared_ptr<Bind::Bindable> bind_in ) noexcept;
	void Submit( const Drawable& drawable ) const;
	void Bind( Graphics& gfx ) const noxnd;
	void InitializeParentReferences( const Drawable& parent ) noexcept;
	void Accept( TechniqueProbe& probe );
	void Link( n_SherbRendergraphpass::RenderGraph& rg );
private:
	std::vector<std::shared_ptr<Bind::Bindable>> bindables;
	n_SherbRendergraphpass::RenderQueuePass* pTargetPass = nullptr;
	std::string targetPassName;
};