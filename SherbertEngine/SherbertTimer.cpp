// ©2021 JDSherbert. All rights reserved.

#include "SherbertTimer.h"

using namespace std::chrono;

SherbertTimer::SherbertTimer() noexcept
{
	last = steady_clock::now();
}

float SherbertTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float SherbertTimer::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}
