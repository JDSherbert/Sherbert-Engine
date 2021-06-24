#include "SherbertEngine_Chronometry.h"

using namespace std::chrono;
//Time.deltaTime
Time::Time() noexcept
{
	last = steady_clock::now();
}

float Time::Mark() noexcept
{
	//Time calc = time between previous frame - current frame
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	deltaTime = frameTime.count();

	return deltaTime;
}

float Time::Peek() const noexcept
{
	return duration<float>( steady_clock::now() - last ).count();
}


