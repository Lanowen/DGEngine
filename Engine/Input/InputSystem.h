#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

#include <vector>
#include "IKeyboardHandler.h"

class InputSystem {
public:
	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();

	void Initialize();
	void Update();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	void RegisterKeyboardHandler(IKeyboardHandler*);

private:
	bool m_keys[256];
	std::vector<IKeyboardHandler*> m_keyboardHandlers;
};

#endif