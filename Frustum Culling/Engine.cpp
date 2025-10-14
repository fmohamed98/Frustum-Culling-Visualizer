#include "Engine.h"
#include "Constants.h"

//constructor
CEngine::CEngine()
	:
	m_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Frustum Culling Demo")  //initialise window
{
	m_window.GetKeyboard().EnableAutoRepeatChars();		//enable continous input on press
	m_graphics = std::make_unique<CGraphics>(m_window.GetWindowHandle());	//create graphics object
}

CEngine::~CEngine()
{
}

int CEngine::Run()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = CWindow::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}


		Update(); //run update
	}
}

//Function that runs for every frame
void CEngine::Update()
{

	UpdateCamera();	 //updates camera movement based on keyboard input

	m_graphics->ClearBuffer(0.0f, 0.0f, 0.0f); //clear backbuffer

	m_graphics->Update();	//primary update function
	m_graphics->Render(); //primary render function

	m_graphics->EndFrame();	//present backbuffer to front buffer

	m_graphics->m_timer.UpdateFPS();	//updates fps each frame
}

//Function that changes camera orientation based on user input
void CEngine::UpdateCamera()
{

	if (m_window.GetKeyboard().KeyIsPressed(FORWARD_KEY))
	{
		m_graphics->m_camera.AdjustPosition(m_graphics->m_camera.GetForwardVector() * CAMERA_SPEED); //move forward
	}
	if (m_window.GetKeyboard().KeyIsPressed(BACKWARD_KEY))
	{
		m_graphics->m_camera.AdjustPosition(m_graphics->m_camera.GetBackwardVector() * CAMERA_SPEED); //move backward
	}
	if (m_window.GetKeyboard().KeyIsPressed(LEFT_KEY))
	{
		m_graphics->m_camera.AdjustPosition(m_graphics->m_camera.GetLeftVector() * CAMERA_SPEED); //move left
	}
	if (m_window.GetKeyboard().KeyIsPressed(RIGHT_KEY))
	{
		m_graphics->m_camera.AdjustPosition(m_graphics->m_camera.GetRightVector() * CAMERA_SPEED); //move right
	}

	if (m_window.GetKeyboard().KeyIsPressed(UP_KEY))
	{
		m_graphics->m_camera.AdjustPosition(DEFAULT_UP_VECTOR * CAMERA_SPEED); //move up
	}
	if (m_window.GetKeyboard().KeyIsPressed(DOWN_KEY))
	{
		m_graphics->m_camera.AdjustPosition(DEFAULT_DOWN_VECTOR * CAMERA_SPEED); //move down
	}

	if (m_window.GetKeyboard().KeyIsPressed(ROTATION_LEFT))
	{
		m_graphics->m_camera.AdjustRotation(0.0f, -ROTATION_SPEED, 0.0f); //rotate left
	}
	if (m_window.GetKeyboard().KeyIsPressed(ROTATION_RIGHT))
	{
		m_graphics->m_camera.AdjustRotation(0.0f, ROTATION_SPEED, 0.0f); //rotate right
	}

}
