#ifndef _ENGINE_H_
#define _ENGINE_H_



#include <Windows.h>
#include "InputSystem.h"
#include "GraphicsSystem.h"
#include "foundation.hpp"

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

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputSystem* m_Input;
	GraphicsSystem* m_Graphics;
	
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Engine* ApplicationHandle = 0;

#endif