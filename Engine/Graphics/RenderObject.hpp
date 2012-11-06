#ifndef _RENDEROBJECT_HPP_
#define _RENDEROBJECT_HPP_

#include "Texture.h"
#include "TextureShader.h"
#include "..\GameObject.hpp"
#include "Exceptions\RenderException.hpp"

static enum ShaderType {
	e_TextureShaded,
	e_ColorShaded,
	e_None
};

class ITexturedShaded {
public:
	virtual int GetIndexCount() = 0;
	virtual ID3D11ShaderResourceView* GetTexture() = 0;
};

class RenderObject : public GameObject {
public:
	RenderObject() {};
	RenderObject(const RenderObject& other) {};
	~RenderObject() {};

	virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

	virtual const ShaderType getShaderType() {
		return ShaderType::e_None;
	}
};

class TexturedRenderObject : public RenderObject, public ITexturedShaded {
public:
	TexturedRenderObject() {};
	TexturedRenderObject(const TexturedRenderObject& other) {};
	~TexturedRenderObject() {};

	virtual const ShaderType getShaderType() {
		return ShaderType::e_TextureShaded;
	}
};



#endif