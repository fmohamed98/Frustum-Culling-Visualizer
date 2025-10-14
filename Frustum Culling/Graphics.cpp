#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "GuiManager.h"
#include <unordered_map>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

bool CGuiManager::m_isCheckBoxOn = false; //set checkbox off on imgui

//constructor
CGraphics::CGraphics(HWND& hWnd)
{
	//set poisition for god view camera
	m_godViewcamera.SetPosition(0.0f, 25.0f, -500.0f);
	m_godViewcamera.SetProjectionValues(FOV / 2, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR, Z_FAR);

	//initialize d3d
	InitD3D(hWnd);

	//initialize shaders
	m_shader.InitPixelShader(m_device.Get());
	m_shader.InitVertexShader(m_device.Get());
	m_shader.InitInputLayout(m_device.Get());

	//create mesh objects
	InitObjects();

	//set Imgui
	CGuiManager::Init(hWnd, m_device.Get(), m_deviceContext.Get());
}

//Function to present backbuffer to frontbuffer
void CGraphics::EndFrame()
{
	m_swapChain->Present(1u, 0u);
}

//Function to clear backbuffer
void CGraphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue,1.0f };
	m_deviceContext->ClearRenderTargetView(m_target.Get(), color);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

//Function to bind d3d objects to the render pipeline for a given mesh & texture
void CGraphics::DrawMesh(CMesh& mesh, CTexture& texture)
{
	//bind vertex buffer
	mesh.BindVertexBuffer(m_deviceContext.Get());
	// Bind index buffer to pipeline
	mesh.BindIndexBuffer(m_deviceContext.Get());
	//bind constant buffer
	mesh.BindConstantBuffer(m_device.Get(), m_deviceContext.Get(), m_godViewcamera.GetViewMatrix() * m_godViewcamera.GetProjectionMatrix());


	// bind pixel shader
	m_shader.BindPixelShader(m_deviceContext.Get());
	// bind vertex shader
	m_shader.BindVertexShader(m_deviceContext.Get());
	// bind vertex layout
	m_shader.BindInputLayout(m_deviceContext.Get());

	//bind texture
	texture.BindTexture(m_deviceContext.Get());
	// bind sampler
	m_deviceContext->PSSetSamplers(0u, 1u, m_sampler.GetAddressOf());

	//bind depth stencil state and view
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1u);
	m_deviceContext->OMSetRenderTargets(1u, m_target.GetAddressOf(), m_depthStencilView.Get());

	// Set primitive topology to triangle list (groups of 3 vertices)
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw mesh according to indices
	mesh.Render(m_deviceContext.Get());
}

//Function to update visibilty of each object based on camera view
void CGraphics::Update()
{
	DirectX::XMMATRIX viewProjectionMatrix = m_camera.GetViewMatrix() * m_camera.GetProjectionMatrix(); //Get view-projection matrix from camera
	m_frustum.UpdatePlanes(viewProjectionMatrix);	//construct new view frustum according to latest camera view

	for (int i = 0;i < ENTITY_COUNT;i++)
	{
		//if culling is switched on
		if (CGuiManager::m_isCheckBoxOn)
		{
			//check if mesh is within view frustum for culling
			if (!m_frustum.CheckBounds(m_entities[i]->GetCenterCoords(), m_entities[i]->GetBoundingBox().GetOffSet()))
			{
				//if the object was within the view frustum before, then rendercount is decremented 
				if (m_entities[i]->m_isInsideFrustum)
				{
					m_entities[i]->m_isInsideFrustum = false;
					m_renderCount--;
				}

			}
			//if object is within the view frustum
			else
			{
				//if the object was outside frustum before, then rendercount is incremented 
				if (!m_entities[i]->m_isInsideFrustum)
				{
					m_entities[i]->m_isInsideFrustum = true;
					m_renderCount++;
				}

			}

		}

		//if culling is switched off
		else
		{
			m_entities[i]->m_isInsideFrustum = true;
			m_renderCount = ENTITY_COUNT;
		}
	}

}

//Function to render level and meshes
void CGraphics::Render()
{

	RenderLevel();
	RenderObjects();

	// configure viewport
	D3D11_VIEWPORT viewPort;
	viewPort.Width = WINDOW_WIDTH;
	viewPort.Height = WINDOW_HEIGHT;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1u, &viewPort);
}

//Function to render level
void CGraphics::RenderLevel()
{
	DrawMesh(*m_baseLevel, *m_baseLevelTexture); //render base level
}

//Function to render the meshes
void CGraphics::RenderObjects()
{
	for (int i = 0; i < ENTITY_COUNT; i++)
	{
		if (m_entities[i]->m_isInsideFrustum)
		{
			DrawMesh(*m_entities[i], *m_entityTexture);	//Render meshes within the view frustum
		}

	}

	//creating a map of all features to be displayed to imgui
	std::unordered_map<std::string, std::string> renderList;
	renderList["FPS : "] = std::to_string(m_timer.GetFPS());
	renderList["Render Count : "] = std::to_string(m_renderCount);

	CGuiManager::Render(renderList);

}

//Function to create level and all the mesh objects
void CGraphics::InitObjects()
{
	//init base level
	m_baseLevel = std::make_unique<CMesh>(DirectX::XMFLOAT3(0, -LEVEL_HEIGHT, 0), "Models\\level.obj");  //create base level object
	m_baseLevelTexture = std::make_unique<CTexture>();	//create texture object for base level
	m_baseLevelTexture->InitTexture("Textures\\brick.png", m_device.Get()); //init texture for base

	//init vertex and index buffers
	m_baseLevel->InitVertexBuffer(m_device.Get());
	m_baseLevel->InitIndexBuffer(m_device.Get());

	//init mesh objects
	for (int i = 0;i < ENTITY_COUNT;i++)
	{
		m_entities[i] = std::make_unique<CEntity>(DirectX::XMFLOAT3(rand() % LEVEL_WIDTH - 500, -(LEVEL_HEIGHT - 1), rand() % LEVEL_DEPTH - 500), "Models\\cube1.obj");
		m_entities[i]->InitVertexBuffer(m_device.Get());
		m_entities[i]->InitIndexBuffer(m_device.Get());
	}

	m_entityTexture = std::make_unique<CTexture>();
	m_entityTexture->InitTexture("Textures\\gold.jpg", m_device.Get());

}

//init d3d objects
void CGraphics::InitD3D(HWND& hWnd)
{
	//create swapchain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// create swap chain
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain,
		&m_device, nullptr, &m_deviceContext);

	// gain access to texture subresource in swap chain (back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_target);


	// Create Sampler State
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_device->CreateSamplerState(&samplerDesc, &m_sampler);


	//create depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	m_device->CreateDepthStencilState(&dsDesc, &m_depthStencilState);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_device->CreateTexture2D(&descDepth, nullptr, &depthStencil);

	//create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	m_device->CreateDepthStencilView(depthStencil.Get(), &descDSV, &m_depthStencilView);

}



