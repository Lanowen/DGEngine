#pragma once

#include <EMath.hpp>
#include <engine.h>
#include <Scripting\ScriptEvent.h>
#include <Scripting\LPCDString.h>
#include <Utils\ScriptWrappers.hpp>
#include <Input\IKeyboardHandler.h>
#include <Graphics\Text.h>

#include "Ship.hpp"
#include "ChangeCameraView.h"

class ShipGame : public IKeyboardHandler {
public:
	ShipGame(){}
	~ShipGame(){
		delete bmFATHER;
		delete m_scene;
		delete m_camera;
		delete m_viewPort;
		delete m_pausedText;
		delete m_beginGameText;
		delete m_infoText;
		delete m_infoText2;

		for (std::vector<Bitmap*>::iterator itr = m_babies.begin(); itr != m_babies.end(); itr++){
			delete *itr;
		}
	}

	void Initialize(){
		gameStarted = paused = false;

		RegisterScriptFunctions();

		ApplicationHandle->scriptManager->ExecuteScript("programdata/ShipGameScript.lua");

		m_viewPort = ApplicationHandle->graphicsSystem->CreateViewport(ApplicationHandle->windowWidth, ApplicationHandle->windowHeight, false, ApplicationHandle->hwnd, false, 1000.0f, 0.1f);
		m_scene = new Scene("mainScene");	

		m_camera = m_scene->createCamera("MainCamera");
		m_camera->position = Vec3(0,0,-200);
		m_camera->rotation = Quat::fromYawPitchRoll(0,3.141592654/4,0);

		m_viewPort->setCamera(m_camera);

		ApplicationHandle->audioSystem->SetListener(m_camera);
		ApplicationHandle->inputSystem->RegisterKeyboardHandler(this);

		m_pausedText = ApplicationHandle->graphicsSystem->CreateText("PAUSED", "pauseText");
		m_pausedText->scale = Vec3(3,3, 1);
		m_pausedText->position = Vec3(-50,200, 0);

		m_beginGameText = ApplicationHandle->graphicsSystem->CreateText("Press ENTER to begin", "beginGameText");
		m_beginGameText->scale = Vec3(3,3, 1);
		m_beginGameText->position = Vec3(-180,200, 0);
		m_scene->getRootGUISceneNode()->AddObject(m_beginGameText);

		m_infoText = ApplicationHandle->graphicsSystem->CreateText("Use up and down to move forward/back and use WSAD to turn, and R to reset your ship", "infoText");
		m_infoText->position = Vec3(-180,100, 0);
		m_scene->getRootGUISceneNode()->AddObject(m_infoText);

		m_infoText2 = ApplicationHandle->graphicsSystem->CreateText("Keys 1, 2, 3, 4 cycle through camera views and press SPACE to pause", "infoText2");
		m_infoText2->position = Vec3(-180,50, 0);
		m_scene->getRootGUISceneNode()->AddObject(m_infoText2);

		bmFATHER = ApplicationHandle->graphicsSystem->CreateBitmap(L"programdata/avatar.jpg", "avatarBMP");
		for(int i = 0; i < 100; i++){
			Bitmap* bm = new Bitmap(*bmFATHER);
			bm->scale = Vec3(0.25, 0.25, 0.25);
			double bound = 500;
			bm->position = Vec3(((rand()%1000+1)/1000.0 - 0.5) *bound,((rand()%1000+1)/1000.0 - 0.5) *bound, ((rand()%1000+1)/1000.0 - 0.5) *bound);
			bm->rotation = Quat::fromYawPitchRoll((rand()%100+1)/100.0 * 3.141592654, (rand()%100+1)/100.0 * 3.141592654,(rand()%100+1)/100.0 * 3.141592654);
			m_scene->getRootSceneNode()->AddObject(bm);
			m_babies.push_back(bm);
		}

		std::shared_ptr<ScriptEvent> evt(new ScriptEvent);
		evt->Name = "CreatePlayer";
		ApplicationHandle->eventManager->TriggerEvent(evt);
	}

	void Update(Real deltaTime){
		ApplicationHandle->inputSystem->Update();
		ApplicationHandle->audioSystem->Update(deltaTime);

		if(!paused && gameStarted){
			ApplicationHandle->scriptManager->BeforeUpdate();
			
			m_scene->Update(deltaTime);			
				
			ApplicationHandle->scriptManager->AfterUpdate();
			
		}
	}

	void Render(){
		m_viewPort->Render();
	}

private:
	void ScriptCreatePlayer(std::string modelName, std::string textureName, std::string gameObjectName)
	{
		wstringstream wss;
		wss << textureName.c_str();
		Ship* ship = new Ship(ApplicationHandle->graphicsSystem, modelName, wss.str(), gameObjectName);

		m_scene->getRootSceneNode()->AddObject(ship);

		ApplicationHandle->inputSystem->RegisterKeyboardHandler(ship);

		ApplicationHandle->audioSystem->PlayMusic("programdata/music.mp3");
	}

	LuaPlus::LuaObject findGameObjectByName(std::string name){
		GameObject* go = m_scene->findGameObjectByName(name);

		if(go){
			return ScriptWrappers::Wrap(go);
		}

		LuaPlus::LuaObject obj;

		obj.AssignNil(LuaManager::Get()->GetState());

		return obj;
	}

	void RegisterScriptFunctions(){
		LuaManager::Get()->GetGlobals().RegisterDirect("CreatePlayer", *this, &ShipGame::ScriptCreatePlayer);
		LuaManager::Get()->GetGlobals().RegisterDirect("findGameObjectByName", *this, &ShipGame::findGameObjectByName);
	}

	virtual bool HandleKeysDown(bool keys[256]){
		if(keys['1'] && ! lastKeys['1']){
			std::shared_ptr<ChangeCameraView> evt(new ChangeCameraView);
			evt->Name = "ChangeCameraView";
			evt->m_view = 1;
			ApplicationHandle->eventManager->TriggerEvent(evt);
		}
		else if(keys['2'] && ! lastKeys['2']){
			std::shared_ptr<ChangeCameraView> evt(new ChangeCameraView);
			evt->Name = "ChangeCameraView";
			evt->m_view = 2;
			ApplicationHandle->eventManager->TriggerEvent(evt);
		}
		else if(keys['3'] && ! lastKeys['3']){
			std::shared_ptr<ChangeCameraView> evt(new ChangeCameraView);
			evt->Name = "ChangeCameraView";
			evt->m_view = 3;
			ApplicationHandle->eventManager->TriggerEvent(evt);
		}
		else if(keys['4'] && ! lastKeys['4']){
			std::shared_ptr<ChangeCameraView> evt(new ChangeCameraView);
			evt->Name = "ChangeCameraView";
			evt->m_view = 4;
			ApplicationHandle->eventManager->TriggerEvent(evt);
		}
		else if(keys['R'] && ! lastKeys['R']){
			std::shared_ptr<ScriptEvent> evt(new ScriptEvent);
			evt->Name = "ResetPlayer";
			ApplicationHandle->eventManager->TriggerEvent(evt);
		}
		else if(keys[VK_SPACE] && !lastKeys[VK_SPACE]){
			if(gameStarted){
				paused = !paused;
				if(paused){
					m_scene->getRootGUISceneNode()->AddObject(m_pausedText);
					m_infoText->UpdateSentence("Press SPACE to unpause");
					m_infoText->position = Vec3(-40, 100,0);
					m_scene->getRootGUISceneNode()->AddObject(m_infoText);
				}
				else {
					m_scene->getRootGUISceneNode()->RemoveObject(m_pausedText);
					m_scene->getRootGUISceneNode()->RemoveObject(m_infoText);
				}
			}
		}
		else if(keys[VK_RETURN] && !lastKeys[VK_RETURN]){
			if(!gameStarted){
				gameStarted = true;
				m_scene->getRootGUISceneNode()->findAndRemoveGameObjectByName("beginGameText");
				m_scene->getRootGUISceneNode()->RemoveObject(m_infoText);
				m_scene->getRootGUISceneNode()->RemoveObject(m_infoText2);
			}
		}

		memcpy(lastKeys, keys, sizeof(lastKeys));

		return true;
	}

private:
	bool lastKeys[256];
	bool paused, gameStarted;
	Scene* m_scene;
	Camera* m_camera;
	ViewPort* m_viewPort;
	Bitmap* bmFATHER;
	Text *m_pausedText, *m_beginGameText, *m_infoText, *m_infoText2;
	std::vector<Bitmap*> m_babies;
};