#ifndef _MODEL_H_
#define _MODEL_H_

#include <D3D11.h>
#include <D3DX10math.h>

#include "Texture.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*, WCHAR*, D3DXVECTOR3);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	Texture* m_texture;
	D3DXVECTOR3 m_position;
};

#endif