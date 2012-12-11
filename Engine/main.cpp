#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow){
	Engine* m_Engine;
	bool result;

	m_Engine = new Engine;
	if(!m_Engine){
		return 0;
	}

	result = m_Engine->Initialize();
	if(result){
		m_Engine->Run();
	}

	m_Engine->Shutdown();

	delete m_Engine;
	m_Engine = 0;

	return 0;
}
