#include <Graphics/Model.h>
#include <Input/IKeyboardHandler.h>
#include <Graphics/GraphicsSystem.h>
#include <functional>
#include <EMath.hpp>
#include <Graphics\GraphicsSystem.h>

class Ship : public Model, public IKeyboardHandler  {
public:
	Ship(GraphicsSystem* graphics, std::string modelName, std::wstring textureName, std::string name) : m_Vel(Vec3(0,0,0)), m_RotVel(Vec3(0,0,0)), Model(*graphics->CreateModel(modelName, textureName, name)) {
		m_rotation = Quat::fromYawPitchRoll(3.141592654/2,0,0);
	}

	virtual ~Ship() {

	}

	virtual void Update(Real deltaTime){
		m_position += m_Vel*deltaTime*10;
		m_rotation *= Quat::fromYawPitchRoll(m_RotVel.x*deltaTime,m_RotVel.y*deltaTime,m_RotVel.z*deltaTime);
	}

	virtual Vec3 getForward() { Vec3 f(-1,0,0); return m_rotation.rotate(f); };

	virtual bool HandleKeysDown(bool keys[256]){

		m_Vel = Vec3(0,0,0);
		m_RotVel = Vec3(0,0,0);

		double multiplier = 1;

		if(keys[VK_SHIFT]){
			multiplier = 2;
		}

		if(keys[VK_DOWN] ){
			m_Vel -= forward*5*multiplier;
		}
		if(keys[VK_UP]){
			m_Vel += forward*5*multiplier;
		}

		if(keys['S']){
			m_RotVel.z -= 10;
		}
		if(keys['W']){
			m_RotVel.z += 10;
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