#pragma once
#include <DirectXMath.h>
#include<vector>
#include<wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include<string>
#include "Vertex.h"

//class to instantiate meshes
class CMesh
{
public:

	CMesh() = delete;
	~CMesh();
	CMesh(DirectX::XMFLOAT3, const std::string fileName);


	void InitVertexBuffer(ID3D11Device*);
	void InitIndexBuffer(ID3D11Device*);

	void BindVertexBuffer(ID3D11DeviceContext*);
	void BindIndexBuffer(ID3D11DeviceContext*);
	void BindConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, DirectX::XMMATRIX);

	void Render(ID3D11DeviceContext*);

	DirectX::XMFLOAT3& GetCenterCoords();


protected:

	struct SConstantBuffer
	{
		DirectX::XMMATRIX transform;	//constant buffer structure
	};

	DirectX::XMFLOAT3 m_centerCoords;  //coordinates of center

	std::vector<SVertex> m_vertices;	//vertices of mesh
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;

	std::vector<unsigned short> m_indices;	//indices for vertex order for mesh
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;


};

