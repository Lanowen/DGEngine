#ifndef _MODEL_H_
#define _MODEL_H_

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>

#include "RenderObject.hpp"

#include "Texture.h"
#include "..\Data\ModelData.h"
#include "..\Data\VertexData.h"

using namespace std;


class Model : public TexturedRenderObject {
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	Model();
	Model(const Model& model);
	~Model();

	bool Initialize(ID3D11Device*, std::shared_ptr<ModelData>, WCHAR*);
	void Shutdown();

	virtual void Render(ID3D11DeviceContext* deviceContext);

	virtual int GetIndexCount();
	virtual ID3D11ShaderResourceView* GetTexture();	

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(std::shared_ptr<ModelData>);
	void ReleaseModel();	

	

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	Texture* m_texture;
	std::shared_ptr<ModelData> m_model;
	
};

#endif