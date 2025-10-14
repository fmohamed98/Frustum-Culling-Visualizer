#include "Entity.h"

CEntity::~CEntity()
{
}

//entity object created with mesh constructor
CEntity::CEntity(DirectX::XMFLOAT3 centerCoords, const std::string fileName) : CMesh(centerCoords, fileName)
{
	m_boundingBox.CreateBoundingBox(m_vertices); //bounding box created from vertices of mesh
}

CBoundingBox& CEntity::GetBoundingBox()
{
	return m_boundingBox;
}


