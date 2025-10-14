#pragma once
#include "Defines.h"
#include "Graphics.h"
#include <optional>
#include <memory>
#include<string>
#include "KeyBoard.h"

//window class
class CWindow
{

public:

	CWindow(int width, int height, const char* name);
	~CWindow();
	CWindow(const CWindow&) = delete;
	CWindow& operator=(const CWindow&) = delete;

	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;

	CKeyboard& GetKeyboard();
	HWND& GetWindowHandle();

	inline const char* GetName() { return m_windowClassName; }
	inline HINSTANCE GetInstance() { return m_hInstance; }

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	static constexpr const char* m_windowClassName = "Window";
	int m_width;
	int m_height;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	CKeyboard m_keyboard;

};
