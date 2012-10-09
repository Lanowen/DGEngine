#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

class InputSystem {
public:
	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif