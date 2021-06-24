#include "SherbertEngine_Exception.h"
#include <sstream>

//Exception Handler script
//Builds an exception from an unordered map via pointer
SherbertEngine_Exception::SherbertEngine_Exception( int line,const char* file ) noexcept
	:
	line( line ),
	file( file )
{}

const char* SherbertEngine_Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* SherbertEngine_Exception::GetType() const noexcept
{
	return "Sherbert Engine Exception";
}

int SherbertEngine_Exception::GetLine() const noexcept
{
	return line;
}

const std::string& SherbertEngine_Exception::GetFile() const noexcept
{
	return file;
}

std::string SherbertEngine_Exception::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}