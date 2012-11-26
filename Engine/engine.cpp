#include "engine.h"

namespace LPCD
{
    inline bool    Match(TypeWrapper<std::string>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline bool    Match(TypeWrapper<std::string&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline bool Match(TypeWrapper<const std::string&>, lua_State* L, int idx)
        {  return lua_type(L, idx) == LUA_TSTRING;  }
    inline std::string Get(TypeWrapper<std::string>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline std::string Get(TypeWrapper<std::string&>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline std::string Get(TypeWrapper<const std::string&>, lua_State* L, int idx)
        {  return static_cast<const char*>(lua_tostring(L, idx));  }
    inline void Push(lua_State* L, std::string& value)
        {  lua_pushstring(L, value.c_str());  }
    inline void Push(lua_State* L, const std::string& value)
        {  lua_pushstring(L, value.c_str());  }
}

Engine::Engine(){
	m_Input = 0;
	m_Graphics = 0;
	m_ResourceLoader = 0;
	m_EventManager = 0;
}

Engine::Engine(const Engine& other){

}

Engine::~Engine(){

}

bool Engine::Initialize(){
	int screenWidth, screenHeight;

	bool result;

	screenWidth = 0;
	screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	m_Input = new InputSystem;
	if(!m_Input){
		return false;
	}

	m_Input->Initialize();

	m_ResourceLoader = new ResourceLoader();
	if(!m_ResourceLoader){
		return false;
	}

	result = m_ResourceLoader->Initialize();
	if(!result){
		return false;
	}

	m_Graphics = new GraphicsSystem;
	if(!m_Graphics){
		return false;
	}

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd, m_ResourceLoader);
	if(!result){
		return false;
	}

	m_EventManager = new EventManager();
	result = m_EventManager->Initialize();
	if(!result){
		return false;
	}

	//m_EventManager->AddEventListener("testEvent", fastdelegate::MakeDelegate(this, &Engine::onEvent));
	//m_EventManager->TriggerEvent(new Event("testEvent"));

	m_ScriptManager = new ScriptManager();
	result = m_ScriptManager->Initialize();
	if(!result){
		return false;
	}

	RegisterScriptFunctions();
	
	m_ScriptManager->ExecuteScript("programdata/required.lua");
	m_ScriptManager->ExecuteScript("programdata/test.lua");

	m_Ship.Initialize(m_Graphics);

	m_Graphics->camera->position = Vec3(0,0,-200);

	m_Input->RegisterKeyboardHandler(&m_Ship);

	m_clock = clock();

	return true;
}

void Engine::RegisterScriptFunctions(){
	LuaManager::Get()->GetGlobals().RegisterDirect("print", &Logging::ScriptDebugMessage);
	//LuaManager::Get()->GetGlobals().RegisterDirect("CreatePlayer", *this, &Engine::ScriptCreatePlayer);
	//LuaManager::Get()->GetGlobals().RegisterDirect("MoveCamera", *this, &Engine::MoveTo);
	//LuaManager::Get()->GetGlobals().RegisterDirect("GetCamera", *this, &Engine::GetCamera);
}

void Engine::Shutdown(){
	if(m_Graphics){
		m_Graphics->Shutdown();
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
			m_Input->Update();
			
			clock_t temp = clock();
			float deltaTime = (temp - m_clock)/(float)CLOCKS_PER_SEC;
			m_clock = temp;

			if(deltaTime != 0){
				m_Ship.update(deltaTime);
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

		m_Graphics->BeginScene();


		/*std::vector<Model*>::iterator itr = m_Models.begin();
		for(;itr != m_Models.end(); itr++){
			m_Graphics->Render(*itr);
		}*/

		//m_Ship.Render();
		m_Graphics->Render(&m_Ship);


		m_Graphics->EndScene();
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

