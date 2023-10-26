// ©2021 JDSherbert. All rights reserved.

#include "SherbertException.h"

#include <sstream>


SherbertException::SherbertException( int line,const char* file ) noexcept
	:
	line( line ),
	file( file )
{}

const char* SherbertException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* SherbertException::GetType() const noexcept
{
	return "Sherbert Exception";
}

int SherbertException::GetLine() const noexcept
{
	return line;
}

const std::string& SherbertException::GetFile() const noexcept
{
	return file;
}

std::string SherbertException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}