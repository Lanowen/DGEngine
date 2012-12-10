#pragma once

#include <Graphics\RenderObject.hpp>
#include <EMath.hpp>

class PlayerObject : public RenderObject
{
public:
	PlayerObject() {};
	PlayerObject(const PlayerObject&) {};
	~PlayerObject() {};

	virtual void Update(Real deltaTime) {};
	virtual void draw() {};
};