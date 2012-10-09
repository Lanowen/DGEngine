#pragma once

#include "EMath.hpp"

class GameObject
{
	protected:
	Vec3 m_position;
	Quat m_rotation;
	GameObject() : m_position(Vec3(0,0,0)), m_rotation(Quat::Identity())  {};
	GameObject(const GameObject& other) {};

	public:
	virtual ~GameObject() {};
	Vec3 getPosition(){ return m_position; };
	Quat getRotation(){ return m_rotation; };

	void setPosition(Vec3 position){ m_position = position; };
	void setRotation(Quat rotation){ m_rotation = rotation; };

	virtual void update(Real deltaTime) = 0;

	//TODO:
	//Make this better, would rather just have position and rotation public.
	PROPERTY_GET_SET(Vec3 position, getPosition, setPosition);
	PROPERTY_GET_SET(Quat rotation, getRotation, setRotation);
};