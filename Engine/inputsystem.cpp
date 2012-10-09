#include "inputsystem.h"

InputSystem::InputSystem(){

}

InputSystem::InputSystem(const InputSystem& other){

}

InputSystem::~InputSystem(){

}

void InputSystem::Initialize(){
	int i;
	
	for(i = 0; i < 256; i++){
		m_keys[i] = false;
	}

	return;
}

void InputSystem::KeyDown(unsigned int input){
	m_keys[input] = true;
}

void InputSystem::KeyUp(unsigned int input){
	m_keys[input] = false;
}

bool InputSystem::IsKeyDown(unsigned int input){
	return m_keys[input];
}