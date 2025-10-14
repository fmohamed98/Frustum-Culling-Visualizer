#include "Frustum.h"

CFrustum::CFrustum()
{
}

CFrustum::~CFrustum()
{
}

//Function to construct the view frustum for each frame from the view-projection matrix returned by the camera
//Plane extraction is done using the Grib/Hartmann method 
//--> adding/subtracting the last column of the view-projection matrix with the other columns
void CFrustum::UpdatePlanes(DirectX::XMMATRIX viewProjectionMatrix)
{

	//left frustum Plane
	//add first column of the matrix to the fourth column
	m_planes[0].a = viewProjectionMatrix.r[0].m128_f32[3] + viewProjectionMatrix.r[0].m128_f32[0];
	m_planes[0].b = viewProjectionMatrix.r[1].m128_f32[3] + viewProjectionMatrix.r[1].m128_f32[0];
	m_planes[0].c = viewProjectionMatrix.r[2].m128_f32[3] + viewProjectionMatrix.r[2].m128_f32[0];
	m_planes[0].d = viewProjectionMatrix.r[3].m128_f32[3] + viewProjectionMatrix.r[3].m128_f32[0];

	// right Frustum Plane
	// subtract first column of matrix from the fourth column
	m_planes[1].a = viewProjectionMatrix.r[0].m128_f32[3] - viewProjectionMatrix.r[0].m128_f32[0];
	m_planes[1].b = viewProjectionMatrix.r[1].m128_f32[3] - viewProjectionMatrix.r[1].m128_f32[0];
	m_planes[1].c = viewProjectionMatrix.r[2].m128_f32[3] - viewProjectionMatrix.r[2].m128_f32[0];
	m_planes[1].d = viewProjectionMatrix.r[3].m128_f32[3] - viewProjectionMatrix.r[3].m128_f32[0];


	// top Frustum Plane
	// subtract second column of matrix from the fourth column
	m_planes[2].a = viewProjectionMatrix.r[0].m128_f32[3] - viewProjectionMatrix.r[0].m128_f32[1];
	m_planes[2].b = viewProjectionMatrix.r[1].m128_f32[3] - viewProjectionMatrix.r[1].m128_f32[1];
	m_planes[2].c = viewProjectionMatrix.r[2].m128_f32[3] - viewProjectionMatrix.r[2].m128_f32[1];
	m_planes[2].d = viewProjectionMatrix.r[3].m128_f32[3] - viewProjectionMatrix.r[3].m128_f32[1];

	// bottom Frustum Plane
	// add second column of the matrix to the fourth column
	m_planes[3].a = viewProjectionMatrix.r[0].m128_f32[3] + viewProjectionMatrix.r[0].m128_f32[1];
	m_planes[3].b = viewProjectionMatrix.r[1].m128_f32[3] + viewProjectionMatrix.r[1].m128_f32[1];
	m_planes[3].c = viewProjectionMatrix.r[2].m128_f32[3] + viewProjectionMatrix.r[2].m128_f32[1];
	m_planes[3].d = viewProjectionMatrix.r[3].m128_f32[3] + viewProjectionMatrix.r[3].m128_f32[1];

	// near Frustum Plane
	//third column of the matrix
	m_planes[4].a = viewProjectionMatrix.r[0].m128_f32[2];
	m_planes[4].b = viewProjectionMatrix.r[1].m128_f32[2];
	m_planes[4].c = viewProjectionMatrix.r[2].m128_f32[2];
	m_planes[4].d = viewProjectionMatrix.r[3].m128_f32[2];

	// far Frustum Plane
	// subtract third column of matrix from the fourth column
	m_planes[5].a = viewProjectionMatrix.r[0].m128_f32[3] - viewProjectionMatrix.r[0].m128_f32[2];
	m_planes[5].b = viewProjectionMatrix.r[1].m128_f32[3] - viewProjectionMatrix.r[1].m128_f32[2];
	m_planes[5].c = viewProjectionMatrix.r[2].m128_f32[3] - viewProjectionMatrix.r[2].m128_f32[2];
	m_planes[5].d = viewProjectionMatrix.r[3].m128_f32[3] - viewProjectionMatrix.r[3].m128_f32[2];


	NormalizePlanes();
}

//Function to normalize plane coefficients
void CFrustum::NormalizePlanes()
{
	//normalize plane coeffcients 

	for (int i = 0; i < 6; ++i)
	{
		float length = sqrt((m_planes[i].a * m_planes[i].a) + (m_planes[i].b * m_planes[i].b) + (m_planes[i].c * m_planes[i].c));
		m_planes[i].a /= length;
		m_planes[i].b /= length;
		m_planes[i].c /= length;
		m_planes[i].d /= length;
	}

}

//Function to check if bounding box to the mesh lies within the frustum view
bool CFrustum::CheckBounds(DirectX::XMFLOAT3 meshCenter, DirectX::XMFLOAT3 boundingBoxOffset)
{
	//bounding box offset is applied to mesh center to get bounding box coordinates and checked if point is within frustum
	//returns true if any of the points is within the view frustum

	//front-lower left point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x - boundingBoxOffset.x, meshCenter.y - boundingBoxOffset.y, meshCenter.z - boundingBoxOffset.z)))
	{
		return true;
	}

	//front-lower right point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x + boundingBoxOffset.x, meshCenter.y - boundingBoxOffset.y, meshCenter.z - boundingBoxOffset.z)))
	{
		return true;
	}

	//front-upper left point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x - boundingBoxOffset.x, meshCenter.y + boundingBoxOffset.y, meshCenter.z - boundingBoxOffset.z)))
	{
		return true;
	}

	//front-upper right point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x + boundingBoxOffset.x, meshCenter.y + boundingBoxOffset.y, meshCenter.z - boundingBoxOffset.z)))
	{
		return true;
	}

	//back-lower left point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x - boundingBoxOffset.x, meshCenter.y - boundingBoxOffset.y, meshCenter.z + boundingBoxOffset.z)))
	{
		return true;
	}

	//back-lower right point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x + boundingBoxOffset.x, meshCenter.y - boundingBoxOffset.y, meshCenter.z + boundingBoxOffset.z)))
	{
		return true;
	}

	//back-upper left point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x - boundingBoxOffset.x, meshCenter.y + boundingBoxOffset.y, meshCenter.z + boundingBoxOffset.z)))
	{
		return true;
	}

	//back-upper right point
	if (IsPointInFrustum(DirectX::XMFLOAT3(meshCenter.x + boundingBoxOffset.x, meshCenter.y + boundingBoxOffset.y, meshCenter.z + boundingBoxOffset.z)))
	{
		return true;
	}

	return false;	//when no point is within the view frustum
}

//Function to check if a given point lies within the frustum view
bool CFrustum::IsPointInFrustum(DirectX::XMFLOAT3 boundingBoxCoords)
{
	//for each of the 6 planes, boundingBoxCoords are checked if they lie within them by taking dot product between plane & position vector

	for (int i = 0;i < 6;i++)
	{
		if (GetPlaneDotCoord(m_planes[i], boundingBoxCoords) < 0.0f)	//if point lies outside a plane
		{
			return false;
		}
	}

	return true;	//returns true if point lies on the right side of all 6 planes (dot product > 0)
}

//Function that returns dot product of plane and a vector coordinate
float CFrustum::GetPlaneDotCoord(SPlane& plane, DirectX::XMFLOAT3 coords)
{
	return plane.a * coords.x + plane.b * coords.y + plane.c * coords.z + plane.d * 1;
}

