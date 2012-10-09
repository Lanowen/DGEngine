#include "Model.h"

Model::Model()
{
	this->m_vertexBuffer = 0;
	this->m_indexBuffer = 0;
	this->m_texture = 0;
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device* device, WCHAR* textureFilename, D3DXVECTOR3 position)
{
	bool result;
	this->m_position = position;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();

	return;
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	this->RenderBuffers(deviceContext);

	return;
}

int Model::GetIndexCount()
{
	return this->m_indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_texture->GetTexture();
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 6;

	m_indexCount = 6;

	vertices = new VertexType[m_vertexCount];

	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	vertices[0].position = D3DXVECTOR3(this->m_position.x + -1.0f, this->m_position.y + -1.0f, this->m_position.z + 0.0f);
	//vertices[0].color = D3DXVECTOR4(255.0f, 0.0f, 0.0f, 1.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(this->m_position.x + -1.0f, this->m_position.y + 1.0f, this->m_position.z + 0.0f);
	//vertices[1].color = D3DXVECTOR4(255.0f, 0.0f, 0.0f, 1.0f);
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[2].position = D3DXVECTOR3(this->m_position.x + 1.0f, this->m_position.y + -1.0f, this->m_position.z + 0.0f);
	//vertices[2].color = D3DXVECTOR4(255.0f, 0.0f, 0.0f, 1.0f);
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(this->m_position.x + -1.0f, this->m_position.y + 1.0f, this->m_position.z + 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(this->m_position.x + 1.0f, this->m_position.y + 1.0f, this->m_position.z + 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(this->m_position.x + 1.0f, this->m_position.y + -1.0f, this->m_position.z + 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
{
	if (m_indexBuffer != 0)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	this->m_texture = new Texture;
	if (!this->m_texture)
	{
		return false;
	}

	result = this->m_texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Model::ReleaseTexture()
{
	if (this->m_texture)
	{
		this->m_texture->Shutdown();
		delete this->m_texture;
		this->m_texture = 0;
	}

	return;
}