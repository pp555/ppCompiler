#ifndef NUMBER_H
#define NUMBER_H

#include <sstream>

class Number : public Element
{
public:
	enum NumberType
	{
		NumInt,
		NumFloat
	};


	Number(int number) : Element(Element::Number), _numberType(NumInt)
	{
		_val.ival = number;
	}
	Number(float number) : Element(Element::Number), _numberType(NumFloat)
	{
		_val.fval = number;
	}
	Number(double number) : Element(Element::Number), _numberType(NumFloat)
	{
		_val.fval = number;
	}

	int ival() const
	{
		return _val.ival;
	}
	float fval() const
	{
		return _val.fval;
	}

	NumberType numberType()
	{
		return _numberType;
	}

	std::string toString()
	{
		std::stringstream stream;
		switch(_numberType)
		{
		case NumInt:
			stream << _val.ival;
			break;
		case NumFloat:
			stream << _val.fval;
			break;
		}

		return stream.str();
	}

private:
	union
	{
		int ival;
		float fval;
	} _val;

	NumberType _numberType;

};

#endif
