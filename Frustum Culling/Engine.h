#pragma once
#include "Window.h"
#include "FPSTimer.h"
#include <memory>

//class to manage the core game loop
class CEngine
{
public:
	CEngine();
	~CEngine();

	int Run();
	void Update();
	void UpdateCamera();

private:

	CWindow m_window;  //window object to initialize window
	std::unique_ptr<CGraphics> m_graphics; //graphics object to render to window

};