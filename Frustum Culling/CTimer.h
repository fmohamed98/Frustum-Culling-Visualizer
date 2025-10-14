#pragma once
#include <chrono>

class CTimer
{
public:

	CTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;

private:

	std::chrono::steady_clock::time_point m_last;
};