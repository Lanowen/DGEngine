#pragma once

#include <string>

class Event
{
public:
	Event();
	Event(std::string);
	Event(const Event&);
	virtual ~Event();
	std::string Name;
};


