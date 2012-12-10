#ifndef _ENGINE_H_
#define _ENGINE_H_


#include <Windows.h>
#include <foundation.hpp>
#include <Input\InputSystem.h>
#include <Graphics\GraphicsSystem.h>
#include <Graphics\Model.h>
#include <ResourceLoader.h>
#include <Event\EventManager.h>
#include <Scripting\ScriptManager.h>
#include <Audio\AudioSystem.h>

#include <time.h>

class Engine {
public:
	Engine();
	Engine(const Engine&);
	~Engine();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void RegisterScriptFunctions();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	int m_screenWidth, m_screenHeight;

	InputSystem* m_Input;
	GraphicsSystem* m_Graphics;
	ResourceLoader* m_ResourceLoader;
	EventManager* m_EventManager;
	ScriptManager* m_ScriptManager;
	AudioSystem* m_AudioSystem;
	clock_t m_clock;

public:
	PROPERTY_GET(HWND hwnd, getHWND);
	PROPERTY_GET(InputSystem* inputSystem, getInputSystem);
	PROPERTY_GET(GraphicsSystem* graphicsSystem, getGraphicsSystem);
	PROPERTY_GET(ResourceLoader* resourceLoader, getResourceLoader);
	PROPERTY_GET(EventManager* eventManager, getEventManager);
	PROPERTY_GET(ScriptManager* scriptManager, getScriptManager);
	PROPERTY_GET(AudioSystem* audioSystem, getAudioSystem);
	PROPERTY_GET(int windowWidth, getWidth);
	PROPERTY_GET(int windowHeight, getHeight);

	InputSystem* getInputSystem(){ return m_Input;}
	GraphicsSystem* getGraphicsSystem(){ return m_Graphics;}
	ResourceLoader* getResourceLoader(){ return m_ResourceLoader;}
	EventManager* getEventManager(){ return m_EventManager;}
	ScriptManager* getScriptManager(){ return m_ScriptManager;}
	AudioSystem* getAudioSystem(){ return m_AudioSystem;}

	HWND getHWND(){return m_hwnd;}

	int getWidth(){return m_screenWidth;}
	int getHeight(){return m_screenHeight;}

	static Engine* g_Engine;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

extern Engine* ApplicationHandle;

#endif