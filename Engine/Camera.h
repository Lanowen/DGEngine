#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "EMath.hpp"
#include "GameObject.hpp"

class Camera : public GameObject
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void Render();
	void GetViewMatrix(Mat44&);

	virtual void update(Real deltaTime) {};


private:
	Mat44 m_viewMatrix;
};

#endif