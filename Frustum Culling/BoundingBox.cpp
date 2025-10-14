#include "BoundingBox.h"
#include<algorithm>

CBoundingBox::CBoundingBox()
{
}

CBoundingBox::~CBoundingBox()
{
}

//Function that calculates x,y,z distances from center of a mesh to it's bounding box
void CBoundingBox::CreateBoundingBox(std::vector<SVertex>& vertices)
{
	//set a minimum and maximum value to find min  &max vertices
	DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < vertices.size();i++)
	{
		//get the smallest vertex 
		minVertex.x = std::min(minVertex.x, vertices[i].position.x);    // find smallest x value in model
		minVertex.y = std::min(minVertex.y, vertices[i].position.y);    // find smallest y value in model
		minVertex.z = std::min(minVertex.z, vertices[i].position.z);    // find smallest z value in model

		//get the largest vertex 
		maxVertex.x = std::max(maxVertex.x, vertices[i].position.x);    // find largest x value in model
		maxVertex.y = std::max(maxVertex.y, vertices[i].position.y);    // find largest y value in model
		maxVertex.z = std::max(maxVertex.z, vertices[i].position.z);    // find largest z value in model
	}

	//find the offsets of mesh from it's centre
	m_offset.x = (maxVertex.x - minVertex.y) / 2.0f;
	m_offset.y = (maxVertex.y - minVertex.y) / 2.0f;
	m_offset.z = (maxVertex.z - minVertex.z) / 2.0f;
}

//Function returns offest vector of bounding box from center of mesh
DirectX::XMFLOAT3 CBoundingBox::GetOffSet()
{
	return m_offset;
}
