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

void InputSystem::Update(){
	std::vector<IKeyboardHandler*>::iterator itr = m_keyboardHandlers.begin();
	for(;itr != m_keyboardHandlers.end(); itr++){
		(*itr)->HandleKeysDown(m_keys);
	}
}

void InputSystem::RegisterKeyboardHandler(IKeyboardHandler* keyHandler){
	m_keyboardHandlers.push_back(keyHandler);
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