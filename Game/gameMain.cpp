//#pragma comment(linker, "/include:ApplicationHandle")

#include <EMath.hpp>
#include <engine.h>
#include <Scripting\ScriptEvent.h>
#include <GameClasses\Ship.hpp>

Engine* ApplicationHandle;

Scene* m_scene;
Camera* m_camera;
ViewPort* m_viewPort;

Ship* m_Ship; //TEMPORARY	

void DGEngineInit(){
	ApplicationHandle->scriptManager->ExecuteScript("programdata/test.lua");


	std::shared_ptr<ScriptEvent> evt(new ScriptEvent);
	evt->Name = "scriptEvent";
	ApplicationHandle->eventManager->TriggerEvent(evt);

	m_viewPort = ApplicationHandle->graphicsSystem->CreateViewport(ApplicationHandle->windowWidth, ApplicationHandle->windowHeight, false, ApplicationHandle->hwnd, false, 1000.0f, 0.1f);

	m_Ship = new Ship(ApplicationHandle->graphicsSystem, "PlayerShip");

	m_Ship->position = Vec3(0,0,0);

	m_scene = new Scene("mainScene");

	Text* text = ApplicationHandle->graphicsSystem->CreateText("Testing", "textText");
	m_scene->getRootGUISceneNode()->addObject(text);

	m_scene->getRootSceneNode()->addObject(m_Ship);

	Bitmap* bm = ApplicationHandle->graphicsSystem->CreateBitmap(L"programdata/avatar.jpg", "avatarBMP");
	//bm->scale = Vec3(0.5f, 0.5f, 0.5f);
	//bm->rotation = Quat::fromYawPitchRoll(0.2, 0,0);
	//m_scene->getRootGUISceneNode()->addObject(bm);

	m_camera = m_scene->createCamera("MainCamera");
	m_camera->position = Vec3(0,0,-200);

	m_viewPort->setCamera(m_camera);

	ApplicationHandle->inputSystem->RegisterKeyboardHandler(m_Ship);

	ApplicationHandle->audioSystem->SetListener(m_camera);

	ApplicationHandle->audioSystem->Play3DSound("programdata/music.mp3", static_cast<GameObject*>(m_Ship));
}

void DGEngineUpdate(Real deltaTime){
	m_scene->Update(deltaTime);
}

void DGEngineRender(){
	m_viewPort->Render();
}

void DGEngineShutDown(){

}