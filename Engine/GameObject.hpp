#ifndef _GAMEOBJECT_HPP_
#define _GAMEOBJECT_HPP_

#include <EMath.hpp>
#include <string>

class GameObject
{
protected:
	Vec3 m_position;
	Quat m_rotation;
	Vec3 m_scale;
	GameObject(std::string name) : m_position(Vec3(0,0,0)), m_rotation(Quat::Identity()), m_scale(Vec3(1,1,1)), m_name(name) {};
	GameObject(const GameObject& other) : m_position(other.m_position), m_rotation(other.m_rotation), m_scale(other.m_scale), m_name(other.m_name) {};

public:
	virtual ~GameObject() {};
	Vec3 getPosition(){ return m_position; };
	Quat getRotation(){ return m_rotation; };
	Vec3 getScale(){ return m_scale; };

	void setPosition(Vec3 position){ m_position = position; };
	void setRotation(Quat rotation){ m_rotation = rotation; };
	void setScale(Vec3 scale){ m_scale = scale; };

	virtual Vec3 getForward() { Vec3 f(0,0,1); return m_rotation.rotate(f); };

	virtual void Update(Real deltaTime){};
	virtual GameObject* findGameObjectByName(std::string findName){
		if(name == findName)
			return this;

		return 0;
	}

	const std::string getName() { return m_name; }
	void setName(std::string name) { m_name = name; }

private:
	std::string m_name;

public:
	//TODO:
	//Make this better, would rather just have position and rotation public.
	PROPERTY_GET_SET(Vec3 position, getPosition, setPosition);
	PROPERTY_GET_SET(Quat rotation, getRotation, setRotation);
	PROPERTY_GET_SET(Vec3 scale, getScale, setScale);

	PROPERTY_GET(Vec3 forward, getForward);
	PROPERTY_GET_SET(std::string name, getName, setName);
};

#endif