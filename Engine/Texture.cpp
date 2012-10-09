#include "Texture.h"

Texture::Texture()
{
	this->m_texture = 0;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &this->m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::Shutdown()
{
	if (this->m_texture)
	{
		this->m_texture->Release();
		this->m_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return this->m_texture;
}