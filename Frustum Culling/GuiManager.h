#pragma once
#include "Defines.h"
#include<d3d11.h>
#include <string>
#include<unordered_map>

//class to manage imgui

class CGuiManager
{
public:


	CGuiManager();
	~CGuiManager();

	static bool m_isCheckBoxOn;	//boolean to status of checkbox

	static void Init(HWND& hWnd, ID3D11Device*, ID3D11DeviceContext*);
	static void Render(std::unordered_map<std::string, std::string>);
};

