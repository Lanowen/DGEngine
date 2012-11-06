#pragma once

#include <exception>


class RenderException : public std::exception {

	virtual const char* what() const throw()
	{
		return "Could not render object";
	}
};