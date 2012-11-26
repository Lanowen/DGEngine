#include "Font.h"


Font::Font()
{
	this->m_Font = 0;
	this->m_Texture = 0;
}


Font::Font(const Font& other)
{
}


Font::~Font()
{
}


bool Font::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	bool result;


	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Font::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();

	return;
}


bool Font::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;


	this->m_Font = new FontType[95];
	if(!this->m_Font)
	{
		return false;
	}

	fin.open(filename);
	if(fin.fail())
	{
		return false;
	}

	for(i=0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> this->m_Font[i].left;
		fin >> this->m_Font[i].right;
		fin >> this->m_Font[i].size;
	}

	fin.close();

	return true;
}


void Font::ReleaseFontData()
{
	if(this->m_Font)
	{
		delete [] this->m_Font;
		this->m_Font = 0;
	}

	return;
}


bool Font::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	this->m_Texture = new Texture;
	if(!this->m_Texture)
	{
		return false;
	}

	result = this->m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Font::ReleaseTexture()
{
	if(this->m_Texture)
	{
		this->m_Texture->Shutdown();
		delete this->m_Texture;
		this->m_Texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* Font::GetTexture()
{
	return this->m_Texture->GetTexture();
}


void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for(i=0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		if(letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + this->m_Font[letter].size), (drawY - 16), 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + this->m_Font[letter].size, drawY, 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + this->m_Font[letter].size), (drawY - 16), 0.0f);
			vertexPtr[index].texture = D3DXVECTOR2(this->m_Font[letter].right, 1.0f);
			index++;

			drawX = drawX + this->m_Font[letter].size + 1.0f;
		}
	}

	return;
}