#ifndef _RENDEROBJECT_HPP_
#define _RENDEROBJECT_HPP_

#include "TextureShader.h"
#include "FontShader.h"
#include <GameObject.hpp>
#include "Exceptions\RenderException.hpp"
//#include "ViewPort.h"
#include "IRenderable.hpp"
#include "IShader.hpp"


class RenderObject : public IRenderable, public GameObject {
public:
	RenderObject(std::string name) : GameObject(name) {}
	RenderObject(const RenderObject& other) : GameObject(other) {}
	~RenderObject() {}
};

class TexturePass : public IShader {
public:
	TexturePass() {};
	TexturePass(const TexturePass& other) {};
	~TexturePass() {};

	static void Initialize(TextureShader* textureShader) {m_TextureShader = textureShader;}

	virtual int GetIndexCount() = 0;
	virtual ID3D11ShaderResourceView* GetTexture() = 0;

	virtual void Shade(RenderObject* ro, ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		Mat44 translation, rotation, scale;

		D3DXMatrixTranslation(&translation, ro->position.x, ro->position.y, ro->position.z);
		D3DXMatrixRotationQuaternion(&rotation, &ro->rotation);
		D3DXMatrixScaling(&scale, ro->scale.x, ro->scale.y, ro->scale.z);

		if(!m_TextureShader->Render(deviceContext, GetIndexCount(), worldMatrix*rotation*translation*scale, viewMatrix, projectionMatrix, GetTexture())){
			throw RenderException();
		}
	}

private:
	static TextureShader* m_TextureShader;
};

class FontPass : public IShader {
public:
	FontPass() {};
	FontPass(const FontPass& other) {};
	~FontPass() {};

	static void Initialize(FontShader* fontShader) {m_FontShader = fontShader;}

	virtual int GetIndexCount() = 0;
	virtual ID3D11ShaderResourceView* GetTexture() = 0;

	virtual void Shade(RenderObject* ro, ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		Mat44 translation, scale;

		D3DXMatrixTranslation(&translation, ro->position.x, ro->position.y, ro->position.z);
		D3DXMatrixScaling(&scale, ro->scale.x, ro->scale.y, ro->scale.z);

		if(!this->m_FontShader->Render(deviceContext, GetIndexCount(), worldMatrix*translation*scale, viewMatrix, projectionMatrix, GetTexture(), pixelColor)){
			throw RenderException();
		}
	}

protected:
	D3DXVECTOR4 pixelColor;

private:
	static FontShader* m_FontShader;
};



#endif