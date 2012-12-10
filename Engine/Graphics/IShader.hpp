#pragma once

#include <D3D11.h>
#include <EMath.hpp>

class RenderObject;

class IShader {
public:
	virtual void Shade(RenderObject* ro, ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix) = 0;
};