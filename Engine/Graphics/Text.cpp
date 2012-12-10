#include "Text.h"

Text::Text(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string text, std::string name) : RenderObject(name)
{
	this->m_Font = 0;

	pixelColor = D3DXVECTOR4(1,1,1,1);

	m_deviceContext = deviceContext;

	bool result;

	this->m_Font = new Font;
	if(!this->m_Font)
	{
		throw std::exception("Failed to create Font object");
	}

	result = this->m_Font->Initialize(device, "programdata/fontdata.txt", L"programdata/font.dds");
	if(!result)
	{
		throw std::exception("Failed to initialize Font object");
	}

	result = InitializeSentence(255, device);
	if(!result)
	{
		throw std::exception("Failed to initialize sentence");
	}

	result = UpdateSentence(text);
	if(!result)
	{
		throw std::exception("Failed to update sentence");
	}
}


Text::Text(const Text& other) : RenderObject(other)
{
	m_Font = other.m_Font;
	m_deviceContext = other.m_deviceContext;
	vertexBuffer = other.vertexBuffer;
	indexBuffer = other.indexBuffer;
	m_vertexCount = other.m_vertexCount;
	m_indexCount = other.m_indexCount;
	m_maxLength = other.m_maxLength;
}


Text::~Text()
{
	this->m_Font->Shutdown();
	delete this->m_Font;
	this->m_Font = 0;

	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}

	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}
}

int Text::GetIndexCount()
{
	return this->m_indexCount;
}

ID3D11ShaderResourceView* Text::GetTexture()
{
	return this->m_Font->GetTexture();
}


void Text::Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix)
{
	unsigned int stride, offset;
    stride = sizeof(VertexType); 
	offset = 0;

	ID3D11DeviceContext* deviceContext = viewPort->GetDeviceContext();

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Shade(this, deviceContext, worldMatrix,viewMatrix, projectionMatrix);
}


bool Text::InitializeSentence(int maxLength,ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	vertexBuffer = 0;
	indexBuffer = 0;

	m_maxLength = maxLength;

	m_vertexCount = 6 * m_maxLength;

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

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
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

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
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


bool Text::UpdateSentence(std::string text){
	int numLetters;
	VertexType* vertices;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	numLetters = text.length();

	if(numLetters > m_maxLength)
	{
		return false;
	}

	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	this->m_Font->BuildVertexArray((void*)vertices, const_cast<char*>(text.c_str()), 0, 0);

	result = m_deviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	m_deviceContext->Unmap(vertexBuffer, 0);

	delete [] vertices;
	vertices = 0;

	return true;
}