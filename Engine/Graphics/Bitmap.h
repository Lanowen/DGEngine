#ifndef INC_BITMAP_H
#define INC_BITMAP_H

#include <D3D11.h>
#include <D3DX10.h>

#include "Texture.h"
#include "RenderObject.hpp"

class Bitmap : public TexturePass, public RenderObject
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
public:
	Bitmap(ID3D11Device* device, WCHAR* textureFilename, std::string name);
	Bitmap(const Bitmap&);
	~Bitmap();
	
	void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		UpdateBuffers(viewPort->GetDeviceContext());
		RenderBuffers( viewPort->GetDeviceContext());
		
		Shade(this, viewPort->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	}

private:
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool RenderToTarget(ID3D11DeviceContext*, int, int);
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;

	bool shared;
};

#endif