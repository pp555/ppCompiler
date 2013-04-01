#ifndef NUMBER_H
#define NUMBER_H

#include <sstream>

class Number : public Element
{
public:
	enum NumberType
	{
		NUM_INT,
		NUM_FLOAT
	};


	Number(int number) : Element(Element::Number), _numberType(NUM_INT), _ival(number)
	{}
	Number(float number) : Element(Element::Number), _numberType(NUM_FLOAT), _fval(number)
	{}
	Number(double number) : Element(Element::Number), _numberType(NUM_FLOAT), _fval(number)
	{}

	int ival() const
	{
		return _ival;
	}
	float fval() const
	{
		return _fval;
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
		case NUM_INT:
			stream << _ival;
			break;
		case NUM_FLOAT:
			stream << _fval;
			break;
		}

		return stream.str();
	}

private:
	int _ival;
	float _fval;

	NumberType _numberType;

};

#endif
