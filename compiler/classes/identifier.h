#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "element.h"
#include <string>

class Identifier : public Element
{
public:
	Identifier(const std::string &name) : Element(Element::Identifier), _name(name)
	{}

	std::string name() const
	{
		return _name;
	}

private:
	std::string _name;
};

#endif
