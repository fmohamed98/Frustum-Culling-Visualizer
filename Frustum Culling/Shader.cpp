#include "Shader.h"
#include <d3dcompiler.h>
#include <sstream>
#include<cassert>

CShader::CShader()
{

}

CShader::~CShader()
{
}

//Function to initialize vertex shader
void CShader::InitVertexShader(ID3D11Device* device)
{
	D3DReadFileToBlob(L"VertexShader.cso", &m_blob); //read from cso file created from hlsl file
	device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader); //create vertex shader
}

//Function to initialize pixel shader
void CShader::InitPixelShader(ID3D11Device* device)
{
	D3DReadFileToBlob(L"PixelShader.cso", &m_blob); // read from cso file created from hlsl file
	device->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader); //create pixel shader
}

//Function to initialize layout
void CShader::InitInputLayout(ID3D11Device* device)
{
	//create layout description
	const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }

	};

	//create layout
	device->CreateInputLayout(
		inputElementDesc, (UINT)std::size(inputElementDesc),
		m_blob->GetBufferPointer(),
		m_blob->GetBufferSize(),
		&m_inputLayout
	);


}

//bind shaders to pipeline
void CShader::BindPixelShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0u);

}

void CShader::BindInputLayout(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(m_inputLayout.Get());
}

void CShader::BindVertexShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0u);
}
