#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <EMath.hpp>
#include "GameObject.hpp"
#include <D3D11.h>
#include "ViewPort.hpp"

class Scene;
//#include "Scene.hpp"

#include <Input\IKeyboardHandler.h>

class Camera : public GameObject
{
	
private:
	friend class Scene;
	Camera(Scene* parent, std::string name);
public:
	
	//Camera(const Camera&);
	~Camera();

	void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 projectionMatrix, Mat44 orthogonalMatrix);
	void GetViewMatrix(Mat44&);

	/*bool operator==(const Camera& other){
		if(this->m_viewMatrix == other.m_viewMatrix)
			return true;

		return false;
	}*/

private:
	Vec3 m_Vel;
	Vec3 m_RotVel;
	Mat44 m_viewMatrix, m_GUIViewMatrix;
	Scene* m_parentScene;
};

#endif