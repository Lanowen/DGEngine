#include "Bitmap.h"
#include <sstream>

Bitmap::Bitmap(ID3D11Device* device, WCHAR* textureFilename, std::string name) : RenderObject(name)
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;

	bool result;

	m_previousPosX = -1;
	m_previousPosY = -1;

	result = InitializeBuffers(device);
	if(!result)
	{
		throw std::exception("Could not initialize Bitmap Buffers");
	}

	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		std::stringstream ss;
		ss << "Could not Load Bitmap Texture: " << textureFilename;
		throw std::exception(ss.str().c_str());
	}
	
	ID3D11Texture2D* res;

	m_Texture->GetTexture()->GetResource((ID3D11Resource**)&res);

	D3D11_TEXTURE2D_DESC desc;

	res->GetDesc(&desc);

	m_bitmapWidth = desc.Width;
	m_bitmapHeight = desc.Height;

	shared = false;
}


Bitmap::Bitmap(const Bitmap& other) : RenderObject(other)
{
	m_vertexBuffer = other.m_vertexBuffer;
	m_indexBuffer = other.m_indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
	m_Texture = other.m_Texture;
	m_bitmapWidth = other.m_bitmapWidth;
	m_bitmapHeight = other.m_bitmapHeight;
	m_previousPosX = other.m_previousPosX;
	m_previousPosY = other.m_previousPosY;

	shared = true;
}


Bitmap::~Bitmap()
{
	if(!shared){
		ReleaseTexture();

		ShutdownBuffers();
	}
}

int Bitmap::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
	return m_Texture->GetTexture();
}

bool Bitmap::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	m_vertexCount = 6;

	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
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
	if(FAILED(result))
	{
		return false;
	}

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Bitmap::ShutdownBuffers()
{
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	if((position.x == m_previousPosX) && (position.y == m_previousPosY))
	{
		return true;
	}

	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	m_previousPosX = position.x;
	m_previousPosY = position.y;

	left = (float)position.x;

	right = left + (float)m_bitmapWidth;

	top = (float)position.y;

	bottom = top - (float)m_bitmapHeight;

	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	vertices[0].position =D3DXVECTOR3(left, top, 0.0f);
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	delete [] vertices;
	vertices = 0;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool Bitmap::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	m_Texture = new Texture;
	if(!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void Bitmap::ReleaseTexture()
{
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}