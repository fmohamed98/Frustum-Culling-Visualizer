#pragma once
#include "Mesh.h"
#include "BoundingBox.h"

//class to handle entities that are to be populated in the level; requires culling
class CEntity : public CMesh
{
private:

	CBoundingBox m_boundingBox;

public:

	~CEntity();
	CEntity(DirectX::XMFLOAT3 centerCoords, const std::string fileName);
	CBoundingBox& GetBoundingBox();

	bool m_isInsideFrustum = true;

};

