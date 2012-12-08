#ifndef _ENGINE_H_
#define _ENGINE_H_


#include <Windows.h>
#include <Input\InputSystem.h>
#include <Graphics\GraphicsSystem.h>
#include <foundation.hpp>
#include <Graphics\Model.h>
#include <ResourceLoader.h>
#include <Event\EventManager.h>
#include <Scripting\ScriptManager.h>
#include <Audio\AudioSystem.h>

#include <time.h>


#include "GameClasses\Ship.hpp"

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

	InputSystem* m_Input;
	GraphicsSystem* m_Graphics;
	ResourceLoader* m_ResourceLoader;
	EventManager* m_EventManager;
	ScriptManager* m_ScriptManager;
	AudioSystem* m_AudioSystem;

	Ship m_Ship; //TEMPORARY
	clock_t m_clock;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Engine* ApplicationHandle = 0;

#endif