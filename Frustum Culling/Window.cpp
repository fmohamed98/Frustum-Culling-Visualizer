#include "Window.h"
#include <sstream>
#include "resource.h"

// Window initialisation
CWindow::CWindow(int width, int height, const char* name)
	:
	m_width(width),
	m_height(height),
	m_hInstance(GetModuleHandle(nullptr))
{

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0
	));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0
	));
	RegisterClassEx(&wc);

	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create window & get hWnd
	m_hWnd = CreateWindow(
		CWindow::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, CWindow::GetInstance(), this
	);
	// check for error

	// newly created windows start off as hidden
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);

}

CWindow::~CWindow()
{
	DestroyWindow(m_hWnd);
}

void CWindow::SetTitle(const std::string& title)
{
	SetWindowText(m_hWnd, title.c_str());

}

std::optional<int> CWindow::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}


CKeyboard& CWindow::GetKeyboard()
{
	return m_keyboard;
}

HWND& CWindow::GetWindowHandle()
{
	return m_hWnd;
}

LRESULT CALLBACK CWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		CWindow* const pWnd = static_cast<CWindow*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&CWindow::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	CWindow* const pWnd = reinterpret_cast<CWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		// stifle this keyboard message if imgui wants to capture

		if (!(lParam & 0x40000000)) // filter autorepeat
		{
			m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:

		m_keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		// stifle this keyboard message if imgui wants to capture

		m_keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
		// clear keystate when window loses focus to prevent input getting "stuck"
	}

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



