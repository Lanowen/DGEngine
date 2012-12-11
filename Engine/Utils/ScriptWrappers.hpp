#pragma once
//#include <LuaPlus\include\LuaPlus.h>
#include <Scripting\LuaManager.h>
#include <EMath.hpp>
#include <GameObject.hpp>
#include <memory>

namespace ScriptWrappers{

static LuaPlus::LuaObject Wrap(GameObject* go);
static LuaPlus::LuaObject Wrap(Vec3 vector);
static LuaPlus::LuaObject Wrap(Quat quat);

class WrappedGameObject {
public:
	WrappedGameObject(GameObject* obj) : m_obj(obj){}
	WrappedGameObject(WrappedGameObject* other) : m_obj(other->m_obj){}
	WrappedGameObject(const WrappedGameObject& other) : m_obj(other.m_obj){}
	~WrappedGameObject(){}

	LuaPlus::LuaObject getPosition(){
		return Wrap(this->m_obj->position);
	}

	LuaPlus::LuaObject getScale(){
		return Wrap(m_obj->scale);
	}

	LuaPlus::LuaObject getRotation(){
		return Wrap(m_obj->rotation);
	}

	LuaPlus::LuaObject getForward(){
		return Wrap(m_obj->forward);
	}

	void setPosition(LUA_NUMBER x, LUA_NUMBER y, LUA_NUMBER z){
		m_obj->position = Vec3(x,y,z);
	}

	void setScale(LUA_NUMBER x, LUA_NUMBER y, LUA_NUMBER z){
		m_obj->scale = Vec3(x,y,z);
	}

	void setRotation(LUA_NUMBER x, LUA_NUMBER y, LUA_NUMBER z, LUA_NUMBER w){
		m_obj->rotation = Quat(x,y,z,w);
	}

	void Dispose(){
		delete this;
	}


private:
	GameObject* m_obj;
};

static LuaPlus::LuaObject Wrap(GameObject* go){
	LuaPlus::LuaObject goObj;
	goObj.AssignNewTable(LuaManager::Get()->GetState());

	WrappedGameObject* wgo = new WrappedGameObject(go);

	//std::shared_ptr<WrappedGameObject> wgo = std::make_shared<WrappedGameObject>(new WrappedGameObject(go));

	goObj.RegisterDirect<WrappedGameObject>("getPosition", *wgo, &WrappedGameObject::getPosition);
	goObj.RegisterDirect<WrappedGameObject>("getScale", *wgo, &WrappedGameObject::getScale);
	goObj.RegisterDirect<WrappedGameObject>("getRotation", *wgo, &WrappedGameObject::getRotation);
	goObj.RegisterDirect<WrappedGameObject>("getForward", *wgo, &WrappedGameObject::getForward);

	goObj.RegisterDirect<WrappedGameObject>("setPosition", *wgo, &WrappedGameObject::setPosition);
	goObj.RegisterDirect<WrappedGameObject>("setScale", *wgo, &WrappedGameObject::setScale);
	goObj.RegisterDirect<WrappedGameObject>("setRotation", *wgo, &WrappedGameObject::setRotation);

	goObj.RegisterDirect<WrappedGameObject>("__gc", *wgo, &WrappedGameObject::Dispose);

	return goObj;
}

static LuaPlus::LuaObject Wrap(Vec3 vector){
	LuaPlus::LuaObject vecObj;
	vecObj.AssignNewTable(LuaManager::Get()->GetState());

	vecObj.SetNumber("x", vector.x);
	vecObj.SetNumber("y", vector.y);
	vecObj.SetNumber("z", vector.z);

	return vecObj;
}

static LuaPlus::LuaObject Wrap(Quat quat){
	LuaPlus::LuaObject quatObj;
	quatObj.AssignNewTable(LuaManager::Get()->GetState());

	quatObj.SetNumber("w", quat.w);
	quatObj.SetNumber("x", quat.x);
	quatObj.SetNumber("y", quat.y);
	quatObj.SetNumber("z", quat.z);

	return quatObj;
}
};