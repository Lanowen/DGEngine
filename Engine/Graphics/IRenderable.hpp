#pragma once

//#include "ViewPort.h"
#include <D3D11.h>
#include <EMath.hpp>
#include "ViewPort.hpp"

class IRenderable {
public:
	virtual void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix) = 0;
};