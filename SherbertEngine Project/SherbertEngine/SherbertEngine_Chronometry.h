#pragma once
#include <chrono>
#include <time.h>
#include <string>

//Time.deltaTime
class Time
{
public:
	Time() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;
	float deltaTime;



private:
	std::chrono::steady_clock::time_point last;
};
