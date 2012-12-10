#include "engine.h"

extern void DGEngineUpdate(Real deltaTime);
extern void DGEngineRender();
extern void DGEngineInit();
extern void DGEngineShutDown();

Engine::Engine(){
	m_Input = 0;
	m_Graphics = 0;
	m_ResourceLoader = 0;
	m_EventManager = 0;
	ApplicationHandle = 0;
}

Engine::Engine(const Engine& other){

}

Engine::~Engine(){

}

bool Engine::Initialize(){

	m_screenWidth = 0;
	m_screenHeight = 0;
	try {

		InitializeWindows(m_screenWidth, m_screenHeight);

		//Use RAII for as much as possible

		m_Input = new InputSystem;
		if(!m_Input){
			return false;
		}

		m_ResourceLoader = new ResourceLoader();
		if(!m_ResourceLoader){
			return false;
		}

		m_Graphics = new GraphicsSystem(m_hwnd, m_ResourceLoader);
		if(!m_Graphics){
			return false;
		}

		m_EventManager = EventManager::Get();
		if(!m_EventManager){
			return false;
		}

		m_AudioSystem = new AudioSystem();
		if(!m_AudioSystem){
			return false;
		}

		m_ScriptManager = new ScriptManager();
		if(!m_ScriptManager){
			return false;
		}

		RegisterScriptFunctions();
	
		m_ScriptManager->ExecuteScript("programdata/required.lua");

		m_clock = clock();

		DGEngineInit();

		return true;
	}
	catch(std::exception exc){
		MessageBoxA(m_hwnd, exc.what(), "Error", MB_OK);
	}

	return false;
}

void Engine::RegisterScriptFunctions(){
	//LuaManager::Get()->GetGlobals().RegisterDirect("findGameObjectByName", &);
}

void Engine::Shutdown(){
	DGEngineShutDown();

	if(m_Graphics){
		delete m_Graphics;
		m_Graphics = 0;
	}

	if(m_Input){
		delete m_Input;
		m_Input = 0;
	}

	ShutdownWindows();

	return;
}

void Engine::Run(){
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	while(!done){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT){
			done = true;
		}
		else{
			
			
			clock_t temp = clock();
			double deltaTime = (temp - m_clock)/(double)CLOCKS_PER_SEC;
			m_clock = temp;

			if(deltaTime != 0){

				m_ScriptManager->BeforeUpdate();
				m_Input->Update();
				m_AudioSystem->Update(deltaTime);
			
				//m_scene->Update(deltaTime);
				DGEngineUpdate(deltaTime);
			
				
				m_ScriptManager->AfterUpdate();

				
			}

			result = Frame();

			if(!result){
				done = true;
			}
		}
	}

	return;
}


bool Engine::Frame(){
	if(m_Input->IsKeyDown(VK_ESCAPE)){
		return false;
	}

	try{
		DGEngineRender();
		//m_viewPort->Render();
	}
	catch(std::exception& e){

		MessageBoxA(m_hwnd, e.what(), "Error", MB_OK);

		return false;
	}

	return true;
}

LRESULT CALLBACK Engine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
	switch(umsg){
	case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int) wparam);
			return 0;
		}

	case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

	default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void Engine::InitializeWindows(int& screenWidth, int& screenHeight){
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;

	m_hinstance = GetModuleHandle(NULL);

	m_applicationName = L"Engine";

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hinstance;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm			= wc.hIcon;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= m_applicationName;
	wc.cbSize			= sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN){
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize			= sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth	= (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight	= (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel	= 32;
		dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else{
		screenWidth		= 800;
		screenHeight	= 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) /2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL,
							m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	ShowCursor(false);

	return;
}

void Engine::ShutdownWindows(){
	ShowCursor(true);

	if(FULL_SCREEN){
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);

	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparm, LPARAM lparam){
	switch(umessage){
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

	default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparm, lparam);
		}
	}
}

