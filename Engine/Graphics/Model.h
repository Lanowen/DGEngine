#ifndef _MODEL_H_
#define _MODEL_H_

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <memory>

#include "RenderObject.hpp"
#include "ViewPort.hpp"

#include "Texture.h"
#include "..\Data\ModelData.h"
#include "..\Data\VertexData.h"

using namespace std;


class Model : private TexturePass, public RenderObject  {
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

public:
	Model(ID3D11Device* device, std::shared_ptr<ModelData> modelData, WCHAR* textureFilename, std::string name);
	Model(const Model& model);
	~Model();

	void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
		RenderBuffers( viewPort->GetDeviceContext());
		
		Shade(this, viewPort->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	}

private:	

	virtual int GetIndexCount();
	virtual ID3D11ShaderResourceView* GetTexture();

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