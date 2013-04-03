#ifndef ADDRESS_HH
#define ADDRESS_HH

#include <string>
#include <sstream>


class Address
{
public:
	Address() : i(-1)
	{}
	int next()
	{
		return ++i;
	}
	int current() const
	{
		return i;
	}
private:
	int i;
};

#endif
