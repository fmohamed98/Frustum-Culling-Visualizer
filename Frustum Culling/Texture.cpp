#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

//Texture initialization from file
void CTexture::InitTexture(const char* fileName, ID3D11Device* device)
{
	// load texture
	int texWidth, texHeight, texNumChannels;
	int texForceNumChannels = 4;
	unsigned char* texTextureBytes = stbi_load(fileName, &texWidth, &texHeight, &texNumChannels, texForceNumChannels);

	int texBytesPerRow = 4 * texWidth;

	// Create Texture
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = texWidth;
	textureDesc.Height = texHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// create subresource data
	D3D11_SUBRESOURCE_DATA sdk = {};
	sdk.pSysMem = texTextureBytes;
	sdk.SysMemPitch = texBytesPerRow;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	device->CreateTexture2D(&textureDesc, &sdk, &pTexture);

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	device->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_textureView);
}

//bind texture resources to pipeline
void CTexture::BindTexture(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShaderResources(0u, 1u, m_textureView.GetAddressOf());
}
