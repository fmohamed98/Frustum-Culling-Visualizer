#pragma once
#include <chrono>
#include<cmath>

//class to calculate fps

class CFPSTimer
{
private:

	std::chrono::high_resolution_clock::time_point m_startTime;
	std::chrono::high_resolution_clock::time_point m_endTime;
	float m_deltaTime;  //time between start time & end time
	float m_frameCount;	//counter for frames
	float m_fps;	//frames counted per second

public:
	CFPSTimer();
	~CFPSTimer();

	void Start();
	void End();
	void SetDeltaTime();
	void UpdateFPS();
	float GetFPS();
};