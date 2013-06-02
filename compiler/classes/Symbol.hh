#ifndef SYMBOL_HH
#define SYMBOL_HH

#include <vector>
#include <cstdlib>

#include "enums.hh"

class Symbol
{
public:
	Symbol() : _type(None), _offset(-1)
	{}
	Symbol(NumberType type, int offset) : _type(type), _offset(offset)
	{}
	
	Symbol(NumberType type, int offset, const std::vector<int> &size) : _type(type), _offset(offset), _size(size)
	{}

	int offset() const {return _offset;}
	NumberType type() const
	{
		if(_type == None)
		{
			std::cerr << "none type detected while getting type of symbol";
			exit(1);
		}
		return _type;
	}

	bool isArray()
	{
		return _size.size() > 0;
	}
	
	std::vector<int> size() const
	{
		return _size;
	}
	
private:
	int _offset;
	NumberType _type;
	std::vector<int> _size;
};

#endif
