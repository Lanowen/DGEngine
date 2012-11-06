#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::Render()
{
	Vec3 up(0,1,0), position, lookAt(0,0,1);
	Mat44 rotationMatrix;

	position = m_position;

	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(&this->m_viewMatrix, &position, &lookAt, &up);

	return;
}

void Camera::GetViewMatrix(Mat44& viewMatrix)
{
	viewMatrix = this->m_viewMatrix;
	return;
}