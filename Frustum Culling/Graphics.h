#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <wrl.h>
#include "Frustum.h"
#include "FPSTimer.h"
#include "Shader.h"
#include "Mesh.h"
#include <memory>
#include <array>
#include "Constants.h"
#include "Camera.h"
#include "Texture.h"
#include "Entity.h"

class CGraphics
{
public:

	CGraphics(HWND& hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics() = default;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

	void Update();
	void Render();
	void RenderLevel();
	void RenderObjects();

	void DrawMesh(CMesh& mesh, CTexture& texture);

	void InitObjects();
	void InitD3D(HWND& hWnd);

	CCamera m_camera;
	CFPSTimer m_timer;

private:

	int m_renderCount = ENTITY_COUNT;

	//COM objects
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;


	//Physical objects to be rendered
	std::array<std::unique_ptr<CEntity>, ENTITY_COUNT>  m_entities;
	std::unique_ptr<CTexture>  m_entityTexture;
	std::unique_ptr<CMesh>  m_baseLevel;
	std::unique_ptr<CTexture>  m_baseLevelTexture;

	CFrustum m_frustum;
	CShader m_shader;
	CCamera m_godViewcamera;	//POV of user

};