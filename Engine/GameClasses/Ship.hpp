#include "../Graphics/Model.h"
#include "../Input/IKeyboardHandler.h"
#include "../Graphics/GraphicsSystem.h"
#include <functional>

class Ship : public Model, public IKeyboardHandler  {
public:
	Ship() : m_Vel(Vec3(0,0,0)), m_RotVel(Vec3(0,0,0)) {

	}

	virtual ~Ship() {

	}

	void Initialize(GraphicsSystem* graphics){
		graphics->LoadModel(this,"programdata/space_frigate_6.modelobj", L"programdata/space_frigate_6_color.jpg");
	}

	virtual void update(Real deltaTime){
		m_position += m_Vel*deltaTime*10;
		m_rotation *= Quat::fromYawPitchRoll(m_RotVel.x*deltaTime,m_RotVel.y*deltaTime,0);
	}

	virtual bool HandleKeysDown(bool keys[256]){

		m_Vel = Vec3(0,0,0);
		m_RotVel = Vec3(0,0,0);

		if(keys[VK_DOWN] && !keys[VK_SHIFT]){
			m_Vel.y -= 10;
		}
		if(keys[VK_UP] && !keys[VK_SHIFT]){
			m_Vel.y += 10;
		}
		if(keys[VK_LEFT]){
			m_Vel.x -= 10;           
		}
		if(keys[VK_RIGHT]){
			m_Vel.x += 10;
		}


		if(keys[VK_DOWN] && keys[VK_SHIFT]){
			m_Vel.z -= 10;
		}
		if(keys[VK_UP] && keys[VK_SHIFT]){
			m_Vel.z += 10;
		}

		if(keys['S']){
			m_RotVel.y -= 10;
		}
		if(keys['W']){
			m_RotVel.y += 10;
		}
		if(keys['A']){
			m_RotVel.x -= 10;           
		}
		if(keys['D']){
			m_RotVel.x += 10;
		}

		return true;
	}

private:
	Vec3 m_Vel;
	Vec3 m_RotVel;
};