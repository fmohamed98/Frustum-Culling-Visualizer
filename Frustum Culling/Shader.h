#pragma once
#include <wrl.h>
#include <d3d11.h>

//class to handle pixel and vertex shaders
class CShader
{
private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:

	CShader();
	~CShader();

	void InitVertexShader(ID3D11Device*);
	void InitPixelShader(ID3D11Device*);
	void InitInputLayout(ID3D11Device*);

	void BindVertexShader(ID3D11DeviceContext*);
	void BindPixelShader(ID3D11DeviceContext*);
	void BindInputLayout(ID3D11DeviceContext*);

};

