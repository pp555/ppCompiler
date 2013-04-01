#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>

class Element
{
public:
	enum Type
	{
		Number,
		Identifier
	};

	Element(int type) : _type((Type)type)
	{}

	Type type() const
	{
		return _type;
	}

	virtual std::string toString() = 0;

private:
	Type _type;
};

#endif //ELEMENT_H
