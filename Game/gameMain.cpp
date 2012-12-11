//#pragma comment(linker, "/include:ApplicationHandle")

#include "GameClasses\ShipGame.h"

void RegisterScriptFunctions();

Engine* ApplicationHandle;

ShipGame* game = new ShipGame();

void DGEngineInit(){
	game->Initialize();
}

void DGEngineUpdate(Real deltaTime){
	game->Update(deltaTime);
}

void DGEngineRender(){
	game->Render();
}

void DGEngineShutDown(){
	delete game;
}