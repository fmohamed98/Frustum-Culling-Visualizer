#include "Mesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


CMesh::~CMesh()
{
}

//constructor : loads a mesh from .obj file and initialised with coordinates of center
CMesh::CMesh(DirectX::XMFLOAT3 coords, const std::string fileName) : m_centerCoords(coords)
{

	Assimp::Importer imp; //assimp object

	const auto model = imp.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices); //load model from obj file
	const auto mesh = model->mMeshes[0]; //retrieve mesh

	//get vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		m_vertices.push_back(
			{
				{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
				{ mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y }
			}
		);
	}

	//get indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		m_indices.push_back(face.mIndices[0]);
		m_indices.push_back(face.mIndices[1]);
		m_indices.push_back(face.mIndices[2]);
	}

}

//Function to create vertex buffer
void CMesh::InitVertexBuffer(ID3D11Device* device)
{
	//create buffer description
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(SVertex) * m_vertices.size();
	bufferDesc.StructureByteStride = sizeof(SVertex);
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = m_vertices.data();

	//create vertex buffer
	device->CreateBuffer(&bufferDesc, &data, &m_vertexBuffer);

}

//Function to create index buffer
void CMesh::InitIndexBuffer(ID3D11Device* device)
{
	//create index buffer description
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = m_indices.size() * sizeof(unsigned short);
	bufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = m_indices.data();

	//create index buffer
	device->CreateBuffer(&bufferDesc, &data, &m_indexBuffer);
}

//Function to bind vertex buffer to pipeline
void CMesh::BindVertexBuffer(ID3D11DeviceContext* deviceContext)
{
	const UINT stride = sizeof(SVertex);
	const UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0u, 1u, m_vertexBuffer.GetAddressOf(), &stride, &offset);

}

//Function to bind index buffer to pipeline
void CMesh::BindIndexBuffer(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

// Function to bind constant buffer to pipeline
void CMesh::BindConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX viewProjectionMatrix)
{
	//create constant buffer object to translate mesh to it's coordinates and move according to camera
	const SConstantBuffer constantBuffer =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixTranslation(m_centerCoords.x,m_centerCoords.y,m_centerCoords.z) *
				viewProjectionMatrix
			)
		}
	};

	//create constant buffer description
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.ByteWidth = sizeof(constantBuffer);
	constantBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &constantBuffer;

	device->CreateBuffer(&constantBufferDesc, &data, &m_constantBuffer);
	deviceContext->VSSetConstantBuffers(0u, 1u, m_constantBuffer.GetAddressOf());	//bind constant buffer
}

//Getter for center
DirectX::XMFLOAT3& CMesh::GetCenterCoords()
{
	return m_centerCoords;
}

void CMesh::Render(ID3D11DeviceContext* deviceContext)
{
	//draw mesh according to indices
	deviceContext->DrawIndexed(m_indices.size(), 0u, 0u);
}
