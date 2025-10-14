#pragma once
#include <DirectXMath.h>

//struct holding plane coefficients
struct SPlane
{
	float a;
	float b;
	float c;
	float d;

};

class CFrustum
{
private:

	SPlane m_planes[6]; //6 planes of frustum

public:

	CFrustum();
	~CFrustum();

	void UpdatePlanes(DirectX::XMMATRIX);
	void NormalizePlanes();
	bool CheckBounds(DirectX::XMFLOAT3, DirectX::XMFLOAT3);
	bool IsPointInFrustum(DirectX::XMFLOAT3);
	float GetPlaneDotCoord(SPlane&, DirectX::XMFLOAT3);
};

