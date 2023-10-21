// ©2021 JDSherbert. All rights reserved.

#pragma once

#include <exception>
#include <string>

class SherbertException : public std::exception
{
public:
	SherbertException( int line,const char* file ) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

protected:
	mutable std::string whatBuffer;

private:
	int line;
	std::string file;
};
