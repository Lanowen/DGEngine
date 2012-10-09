#include "Texture.h"
#include "Model.h"
#include "TextureShader.h"
#include "GameObject.hpp"


class RenderObject : protected GameObject
{
public:
	RenderObject() {};
	RenderObject(const RenderObject&) : GameObject(*this) {};
	~RenderObject() {};

	virtual void draw() = 0;
};