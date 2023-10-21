// ©2021 JDSherbert. All rights reserved.

#pragma once

#include "SherbertException.h"

class ModelException : public SherbertException
{
public:
	ModelException( int line,const char* file,std::string note ) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};
