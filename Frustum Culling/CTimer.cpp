#include "CTimer.h"

using namespace std::chrono;

CTimer::CTimer() noexcept
{
	m_last = steady_clock::now();
}

float CTimer::Mark() noexcept
{
	const auto old = m_last;
	m_last = steady_clock::now();
	const duration<float> frameTime = m_last - old;
	return frameTime.count();
}

float CTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - m_last).count();
}
