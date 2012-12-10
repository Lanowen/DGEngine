#ifndef INC_TEXT_H
#define INC_TEXT_H

#include "Font.h"
//#include "FontShader.h"
#include "RenderObject.hpp"
#include "ViewPort.hpp"
#include <EMath.hpp>


class Text : public FontPass, public RenderObject
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Text(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string text, std::string name);
	Text(const Text&);
	~Text();

	void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix);

	bool UpdateSentence(std::string);

private:
	bool InitializeSentence(int, ID3D11Device*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	Font* m_Font;
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int m_vertexCount, m_indexCount, m_maxLength;
	ID3D11DeviceContext* m_deviceContext;
};

#endif