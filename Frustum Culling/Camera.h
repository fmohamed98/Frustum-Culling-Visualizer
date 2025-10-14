#pragma once
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

//class for handling camera movement
class CCamera
{
public:
	CCamera();
	~CCamera();

	//setters and adjust functions
	void SetPosition(float x, float y, float z);
	void AdjustPosition(XMVECTOR offest);
	void SetRotation(float xRot, float yRot, float zRot);
	void AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset);
	void SetProjectionValues(float FOV, float width, float height, float nearZ, float farZ);

	//getters
	XMVECTOR GetPosition();
	XMFLOAT3 GetRotation();

	const XMMATRIX GetViewMatrix();
	const XMMATRIX GetProjectionMatrix();

	//getters for direction vectors
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();

	void UpdateViewMatrix();

private:

	//rotation and position
	XMFLOAT3 m_rotation;
	XMVECTOR m_position;

	//view matrix and projection matrix
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;

	//direction vectors
	XMVECTOR m_forwardVector;
	XMVECTOR m_backwardVector;
	XMVECTOR m_leftVector;
	XMVECTOR m_rightVector;

};;

