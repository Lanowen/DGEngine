#ifndef _RENDEROBJECT_HPP_
#define _RENDEROBJECT_HPP_

#include "TextureShader.h"
#include <GameObject.hpp>
#include "Exceptions\RenderException.hpp"
#include <functional>
#include "D3DRenderer.h"
#include "IRenderable.hpp"

template<class shader, class... otherShaders>
class RenderObject : public IRenderable, public GameObject,	private shader, private otherShaders... {
public:
	RenderObject() {}
	RenderObject(const RenderObject& other) {}
	~RenderObject() {}

	//virtual bool Render() { RenderToTarget(m_D3D->GetDeviceContext()); return true; }

	virtual bool ShadeToTarget(ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix) { return runShaders<shader, otherShaders...>(deviceContext, worldMatrix, viewMatrix, projectionMatrix); }

	void setRenderTarget(D3DRenderer* D3D) { m_D3D = D3D; }

private:

	template<class s1>
	bool runShaders(ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		return s1::Shade(this, deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	}

	template<class s1, class s2, class... sX>
	bool runShaders(ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		return runShaders<s1>(deviceContext, worldMatrix, viewMatrix, projectionMatrix) && runShaders<s2, sX...>(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	}

private:
	D3DRenderer* m_D3D;
};

class TexturePass{
public:
	TexturePass() {};
	TexturePass(const TexturePass& other) {};
	~TexturePass() {};

	static void Initialize(TextureShader* textureShader) {m_TextureShader = textureShader;}

	virtual int GetIndexCount() = 0;
	virtual ID3D11ShaderResourceView* GetTexture() = 0;

	bool Shade(RenderObject<TexturePass>* ro, ID3D11DeviceContext* deviceContext, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		D3DXMATRIX translation, rotation;

		D3DXMatrixTranslation(&translation, ro->position.x, ro->position.y, ro->position.z);
		D3DXMatrixRotationQuaternion(&rotation, &ro->rotation);

		return m_TextureShader->Render(deviceContext, GetIndexCount(), worldMatrix*rotation*translation, viewMatrix, projectionMatrix, GetTexture());
	}

private:
	static TextureShader* m_TextureShader;
};

#endif