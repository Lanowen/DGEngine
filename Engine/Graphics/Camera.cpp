#include "Camera.h"
#include "Scene.hpp"

Camera::Camera(Scene* parent, std::string name) : m_parentScene(parent), GameObject(name)
{
	Vec3 up(0,1,0), position(0,0,-100), lookAt(0,0,1);
	Mat44 rotationMatrix;

	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);

	lookAt = position + lookAt;
	D3DXMatrixLookAtLH(&this->m_GUIViewMatrix, &position, &lookAt, &up);
}

//Camera::Camera(const Camera& other)
//{
//}

Camera::~Camera()
{
}

void Camera::Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 projectionMatrix, Mat44 orthogonalMatrix)
{
	Vec3 up(0,1,0), lookAt(0,0,1);
	Mat44 rotationMatrix;

	D3DXMatrixRotationQuaternion(&rotationMatrix, &m_rotation);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	lookAt = position + lookAt;
	D3DXMatrixLookAtLH(&this->m_viewMatrix, &position, &lookAt, &up);

	m_parentScene->Render(viewPort, worldMatrix, m_viewMatrix, projectionMatrix);

	m_parentScene->RenderGUI(viewPort, worldMatrix, m_GUIViewMatrix, orthogonalMatrix); // Seems like a sloppy fix

	return;
}

void Camera::GetViewMatrix(Mat44& viewMatrix)
{
	viewMatrix = this->m_viewMatrix;
	return;
}