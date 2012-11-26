#ifndef INC_LOGGING_H
#define INC_LOGGING_H

#include <Windows.h>
#include <sstream>
#include <string>
#include <iostream>

class Logging
{
public:
	static void ThrowError(std::wstring message);
	static void ThrowError(std::wstring message, std::wstring title);
	static void ThrowWarning(std::wstring message);
	static void ThrowWarning(std::string message);
	static void ScriptDebugMessage(std::string message);
	static void SetHWND(HWND);

private:
	static std::wstring ToUtf8(std::string);
	static HWND hwnd;
};

#endif