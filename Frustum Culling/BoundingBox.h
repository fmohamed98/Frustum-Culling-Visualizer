#pragma once
#include<vector>
#include "Vertex.h"

//class for calculating bounding box of a mesh
class CBoundingBox
{
private:

	DirectX::XMFLOAT3 m_offset; //bounding box offset from center of mesh

public:

	CBoundingBox();
	~CBoundingBox();

	void CreateBoundingBox(std::vector<SVertex>&);
	DirectX::XMFLOAT3 GetOffSet();
};

