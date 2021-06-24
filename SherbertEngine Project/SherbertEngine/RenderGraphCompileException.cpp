#include "RenderGraphCompileException.h"
#include <sstream>


namespace n_SherbRendergraphpass
{
	RenderGraphCompileException::RenderGraphCompileException( std::string message,int line,const char* file ) noexcept
		:
		SherbertEngine_Exception( line,file ),
		message( std::move( message ) )
	{}

	const char* RenderGraphCompileException::what() const noexcept
	{
		std::ostringstream oss;
		oss << SherbertEngine_Exception::what() << std::endl;
		oss << "[message]" << std::endl;
		oss << message;
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* RenderGraphCompileException::GetType() const noexcept
	{
		return "Render Graph Compile Exception";
	}


	const std::string& RenderGraphCompileException::GetMessage() const noexcept
	{
		return message;
	}
}