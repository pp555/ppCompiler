#ifndef NUMBER_H
#define NUMBER_H

class Number : public Element
{
public:
	enum NumberType
	{
		NUM_INT,
		NUM_FLOAT
	} _numberType;


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



private:
	int _ival;
	float _fval;

};

#endif
