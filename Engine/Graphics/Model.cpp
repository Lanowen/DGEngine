#include "Model.h"
#include <sstream>

Model::Model(ID3D11Device* device, std::shared_ptr<ModelData> modelData, WCHAR* textureFilename, std::string name) : RenderObject(name)
{
	this->m_vertexBuffer = 0;
	this->m_indexBuffer = 0;
	this->m_texture = 0;
	this->m_model = 0;

	bool result;

	result = LoadModel(modelData);
	if (!result)
	{
		throw std::exception("Could not load modelData");
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		throw std::exception("Could not initialize Model Buffers");
	}

	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		std::stringstream ss;
		ss << "Could not load Model Texture: " << textureFilename;
		throw std::exception(ss.str().c_str());
	}
}

Model::Model(const Model& other) : RenderObject(other)
{
	this->m_vertexBuffer = other.m_vertexBuffer;
	this->m_indexBuffer = other.m_indexBuffer;
	this->m_texture = other.m_texture;
	this->m_model = other.m_model;
	this->m_indexCount = other.m_indexCount;
	this->m_vertexCount = other.m_vertexCount;
}

Model::~Model()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();

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
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	this->m_vertexCount = this->m_model->vertices.size();
	this->m_indexCount = this->m_model->indices.size();

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData) * this->m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &this->m_model->vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * this->m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &this->m_model->indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

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

	stride = sizeof(VertexData);
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

bool Model::LoadModel(std::shared_ptr<ModelData> modelData)
{
	this->m_model = modelData;

	return true;
}

void Model::ReleaseModel()
{
	if (this->m_model)
	{
		this->m_model = 0;
	}

	return;
}