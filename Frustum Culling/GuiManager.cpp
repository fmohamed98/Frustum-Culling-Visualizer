#include "GuiManager.h"
#include<string>
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_impl_dx11.h"

CGuiManager::CGuiManager()
{

}

CGuiManager::~CGuiManager()
{
}

//Function to initialise imgui
void CGuiManager::Init(HWND& hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//initialise ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();
}

//Function to render gui to window given the map of items to be displayed
void CGuiManager::Render(std::unordered_map<std::string, std::string> renderList)
{

	//start new frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("GUI");

	//get items for display

	for (auto& item : renderList)
	{
		std::string stat = item.first + item.second;
		ImGui::Text(stat.c_str());
	}

	ImGui::Checkbox("Do Culling", &m_isCheckBoxOn);


	//end frame
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


}

