#pragma once

#include "RenderObject.hpp"
#include <unordered_map>
#include <string>
#include "ViewPort.hpp"

class SceneNode : public RenderObject {
public:
	SceneNode(std::string name) : RenderObject(name) {}
	SceneNode(const SceneNode& other) : RenderObject(other) {};
	virtual ~SceneNode() {};

	void Update(double deltaTime){
		for (RenderObjectsMap::iterator itr = m_renderObjects.begin(); itr != m_renderObjects.end(); itr++)
		{
			(*itr)->Update(deltaTime);
		}

		for (SceneNodesMap::iterator itr = m_sceneNodes.begin(); itr != m_sceneNodes.end(); itr++)
		{
			(*itr)->Update(deltaTime);
		}
	}

	virtual void Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix) {

		Mat44 translationMatrix, rotationMatrix;

		D3DXMatrixTranslation(&translationMatrix, position.x, position.y, position.z);
		D3DXMatrixRotationQuaternion(&rotationMatrix, &rotation);

		worldMatrix = worldMatrix*rotationMatrix*translationMatrix;

		for (RenderObjectsMap::iterator itr = m_renderObjects.begin(); itr != m_renderObjects.end(); itr++){
			(*itr)->Render(viewPort, worldMatrix, viewMatrix, projectionMatrix);
		}

		for (SceneNodesMap::iterator itr = m_sceneNodes.begin(); itr != m_sceneNodes.end(); itr++)
		{
			(*itr)->Render(viewPort, worldMatrix, viewMatrix, projectionMatrix);
		}
	}

	void AddObject(SceneNode* ro){
		m_sceneNodes.push_back(ro);
	}

	void AddObject(RenderObject* ro){
		m_renderObjects.push_back(ro);
	}

	bool RemoveObject(RenderObject* ro){
		for (RenderObjectsMap::iterator itr = m_renderObjects.begin(); itr != m_renderObjects.end(); itr++){
			if((*itr) == ro){
				m_renderObjects.erase(itr);
				return true;
			}
		}

		for (SceneNodesMap::iterator itr = m_sceneNodes.begin(); itr != m_sceneNodes.end(); itr++)
		{
			if((*itr)->RemoveObject(ro))
				return true;
		}

		return false;
	}

	virtual GameObject* findGameObjectByName(std::string findName){
		GameObject* res = 0;
		res = GameObject::findGameObjectByName(findName);
		if(res)
			return res;

		for (RenderObjectsMap::iterator itr = m_renderObjects.begin(); itr != m_renderObjects.end(); itr++)
		{
			if(res = (*itr)->findGameObjectByName(findName))
				break;
		}

		if(res)
			return res;

		for (SceneNodesMap::iterator itr = m_sceneNodes.begin(); itr != m_sceneNodes.end(); itr++)
		{
			if(res = (*itr)->findGameObjectByName(findName))
				break;
		}

		return res;
	}

	bool findAndRemoveGameObjectByName(std::string findName){
		for (RenderObjectsMap::iterator itr = m_renderObjects.begin(); itr != m_renderObjects.end(); itr++){
			if((*itr)->name == findName){
				m_renderObjects.erase(itr);
				return true;
			}
		}

		for (SceneNodesMap::iterator itr = m_sceneNodes.begin(); itr != m_sceneNodes.end(); itr++)
		{
			if((*itr)->findAndRemoveGameObjectByName(findName))
				return true;
		}

		return false;
	}
	
private:
	//typedef std::unordered_map<std::string, RenderObject*> RenderObjectsMap;
	typedef std::vector<SceneNode*> SceneNodesMap;
	typedef std::vector<RenderObject*> RenderObjectsMap;
	RenderObjectsMap m_renderObjects;
	SceneNodesMap m_sceneNodes;
};