#include "Logging.h"

void Logging::ThrowError(std::wstring message)
{
	MessageBox(hwnd, message.c_str(), L"ERROR!", MB_OK);
}

void Logging::ThrowError(std::wstring message, std::wstring title)
{
	MessageBox(hwnd, message.c_str(), title.c_str(), MB_OK);
}

void Logging::ThrowWarning(std::wstring message)
{
	OutputDebugString(message.c_str());
}

void Logging::ThrowWarning(std::string message)
{
	OutputDebugString(ToUtf8("Overlawled3D WARNING: " + message + "\n").c_str());
}

void Logging::ScriptDebugMessage(std::string message)
{
	OutputDebugString(ToUtf8("Overlawled3D LUA Debug: " + message + "\n").c_str());
}

std::wstring Logging::ToUtf8(std::string in)
{
	std::wstringstream wss;
	wss << in.c_str();

	return wss.str();
}

void Logging::SetHWND(HWND in)
{
	hwnd = in;
}

HWND Logging::hwnd;