// ©2021 JDSherbert. All rights reserved.

#pragma once

#include <chrono>

class SherbertTimer
{
public:
	SherbertTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;

private:
	std::chrono::steady_clock::time_point last;
};