#include "Scene.hpp"

void Scene::Update(double deltaTime){
	for(std::unordered_map<std::string, Camera*>::iterator itr = m_Cameras.begin(); itr != m_Cameras.end(); itr++){
		(*itr).second->Update(deltaTime);
	}
	m_rootSceneNode.Update(deltaTime);
	m_rootGUISceneNode.Update(deltaTime);
}

void Scene::Render(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 projectionMatrix){
	m_rootSceneNode.Render(viewPort, worldMatrix, viewMatrix, projectionMatrix);
}

void Scene::RenderGUI(ViewPort* viewPort, Mat44 worldMatrix, Mat44 viewMatrix, Mat44 orthogonalMatrix){
	viewPort->TurnZBufferOff();
	m_rootGUISceneNode.Render(viewPort, worldMatrix, viewMatrix, orthogonalMatrix);
	viewPort->TurnZBufferOn();
}