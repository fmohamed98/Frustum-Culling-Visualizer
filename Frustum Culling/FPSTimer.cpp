#include "FPSTimer.h"

//constructor : starts the clock
CFPSTimer::CFPSTimer()
{
	m_deltaTime = 0;
	m_frameCount = 0;
	m_fps = 0;
	Start();
	End();
}

CFPSTimer::~CFPSTimer()
{
}

//Function to set start time
void CFPSTimer::Start()
{
	m_startTime = std::chrono::high_resolution_clock::now();
}

//Function to set end time
void CFPSTimer::End()
{
	m_endTime = std::chrono::high_resolution_clock::now();
}

//Function that sets time between start of clock and end time of current frame
void CFPSTimer::SetDeltaTime()
{
	std::chrono::microseconds deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);
	m_deltaTime = (float)(deltaTime).count() / std::pow(10, 6); //finding delta time in seconds

}

//Function to update fps each second
void CFPSTimer::UpdateFPS()
{
	End();  //set end time
	SetDeltaTime();	//find total time since start time

	//if total time exceeds 1 second m_fps is updated with frame count
	if (m_deltaTime >= 1.0f)
	{
		m_fps = m_frameCount;
		m_frameCount = 0;
		Start();	//start clock again
	}

	//if total time < 1 second, frame count is incremented
	else
	{
		m_frameCount++;
	}
}

//Getter for fps
float CFPSTimer::GetFPS()
{
	return m_fps;
}
