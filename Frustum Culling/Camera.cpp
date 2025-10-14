#include "Camera.h"
#include "Constants.h"

//constructor
CCamera::CCamera()
{
	//Sets default position and camera parameters
	SetPosition(0.0f, 0.0f, 0.0f);
	SetProjectionValues(FOV, WINDOW_WIDTH, WINDOW_HEIGHT, Z_NEAR, Z_FAR);

}

CCamera::~CCamera()
{
}


void CCamera::SetPosition(float x, float y, float z)
{
	//Sets position
	XMFLOAT3 position;
	position.x = x;
	position.y = y;
	position.z = z;

	m_position = XMLoadFloat3(&position);
	UpdateViewMatrix();
}

void CCamera::AdjustPosition(XMVECTOR offset)
{
	//increments position by offset
	m_position += offset;
	UpdateViewMatrix();
}

void CCamera::SetRotation(float xRot, float yRot, float zRot)
{
	//sets rotation in radians
	m_rotation.x = xRot / 180 * 3.14159;
	m_rotation.y = yRot / 180 * 3.14159;
	m_rotation.z = zRot / 180 * 3.14159;

	UpdateViewMatrix();
}

void CCamera::AdjustRotation(float xRotOffset, float yRotOffset, float zRotOffset)
{
	//increments rotation by offset
	m_rotation.x += xRotOffset;
	m_rotation.y += yRotOffset;
	m_rotation.z += zRotOffset;

	UpdateViewMatrix();
}

XMVECTOR CCamera::GetPosition()
{
	return m_position;
}

XMFLOAT3 CCamera::GetRotation()
{
	return m_rotation;
}


const XMMATRIX CCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

const XMMATRIX CCamera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void CCamera::SetProjectionValues(float FOV, float width, float height, float nearZ, float farZ)
{
	//creates projection matrix
	m_projectionMatrix = XMMatrixPerspectiveFovLH((FOV / 180) * 3.14f, (float)width / height, nearZ, farZ); //90 degree field of view
}



const XMVECTOR& CCamera::GetForwardVector()
{
	return m_forwardVector;
}

const XMVECTOR& CCamera::GetRightVector()
{
	return m_rightVector;
}

const XMVECTOR& CCamera::GetBackwardVector()
{
	return m_backwardVector;
}

const XMVECTOR& CCamera::GetLeftVector()
{
	return m_leftVector;

}

//updates view matrix and also updates the movement vectors
void CCamera::UpdateViewMatrix()
{
	//calculate camera rotation matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);

	//calculate unit vector of cam target based off camera forward value transformed by cam rotation matrix
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	//adjust cam target to be offset by the camera's current position
	camTarget += m_position;

	//calculate up direction based on current rotation
	XMVECTOR upDirection = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);

	//calculate view matrix
	m_viewMatrix = XMMatrixLookAtLH(m_position, camTarget, upDirection);

	//transforming present direction vectors based on present camera view
	m_forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	m_backwardVector = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, camRotationMatrix);
	m_leftVector = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, camRotationMatrix);
	m_rightVector = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, camRotationMatrix);

}


