#pragma once
#include "SherbertEngine_Exception.h"

#define RGC_EXCEPTION( message ) RenderGraphCompileException( (message),__LINE__,__FILE__ )

namespace n_SherbRendergraphpass
{
	class RenderGraphCompileException : public SherbertEngine_Exception
	{
	public:
		RenderGraphCompileException( std::string message,int line,const char* file ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		const std::string& GetMessage() const noexcept;
	private:
		std::string message;
	};
}