#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <D3D11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif