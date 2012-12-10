#pragma once

//#include "RenderObject.hpp"
#include "IRenderable.hpp"
#include "Camera.h"
#include <unordered_map>
#include "SceneNode.hpp"
#include <string>
#include "ViewPort.hpp"

class GraphicsSystem;

//#include "GraphicsSystem.h"

class Scene {
	friend class Camera;
public:
	Scene(std::string namePrefix) : m_rootSceneNode(namePrefix+"_rootSceneNode"), m_rootGUISceneNode(namePrefix+"_rootGUISceneNode") {}
	Scene(const Scene& other) :m_rootSceneNode(other.m_rootSceneNode), m_rootGUISceneNode(other.m_rootGUISceneNode), m_Cameras(other.m_Cameras) {};
	virtual ~Scene(){};

	Camera* GetCamera(std::string cameraName){ 
		return m_Cameras[cameraName]; 
	}

	Camera* createCamera(std::string cameraName){
		Camera* cam = new Camera(this, cameraName);
		m_Cameras[cameraName] = cam;

		return cam;
	}

	SceneNode* getRootSceneNode(){ return &m_rootSceneNode;}
	SceneNode* getRootGUISceneNode(){ return &m_rootGUISceneNode;}

	GameObject* findGameObjectByName(std::string name){
		for(std::unordered_map<std::string, Camera*>::iterator itr = m_Cameras.begin(); itr != m_Cameras.end(); itr++){
			if((*itr).second->name == name)
				return (*itr).second;
		}
		GameObject* res = m_rootSceneNode.findGameObjectByName(name);

		if(!res)
			res = m_rootGUISceneNode.findGameObjectByName(name);

		return res;
	}

	void Update(double deltaTime);

private:

	virtual void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix);
	virtual void RenderGUI(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 orthogonalMatrix);

private:
	std::unordered_map<std::string, Camera*> m_Cameras;
	SceneNode m_rootSceneNode;
	SceneNode m_rootGUISceneNode;
};