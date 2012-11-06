#ifndef INC_KEYBOARDHANDLER_H
#define INC_KEYBOARDHANDLER_H

class IKeyboardHandler{
public:
	virtual bool HandleKeysDown(bool keys[256]) = 0;
};

#endif