#pragma once
#include <wrl.h>
#include <d3d11.h>
#include<string>

//class to manage texture to mesh objects
class CTexture
{
private:

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

public:

	CTexture();
	~CTexture();

	void InitTexture(const char* fileName, ID3D11Device* device);
	void BindTexture(ID3D11DeviceContext* deviceContext);
};

