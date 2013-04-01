#ifndef ELEMENT_H
#define ELEMENT_H

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

private:
	Type _type;
};

#endif //ELEMENT_H
