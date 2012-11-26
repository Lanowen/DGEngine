#pragma once

#include "D3DRenderer.h"

class IRenderable {
public:
	virtual void RenderToTarget(ID3D11DeviceContext* deviceContext) = 0;
	virtual bool ShadeToTarget(ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix) = 0;
};