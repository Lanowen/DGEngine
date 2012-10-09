#include "RenderObject.hpp"

class PlayerObject : public RenderObject
{
public:
	PlayerObject() {};
	PlayerObject(const PlayerObject&) {};
	~PlayerObject() {};

	virtual void update(Real deltaTime) {};
	virtual void draw() {};
};