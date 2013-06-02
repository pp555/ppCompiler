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
		++i;
		curr = i;
		return curr;
	}
	int next(int size)
	{
		++i;
		curr = i;
		i += size - 1;
		return curr;
	}
	int current() const
	{
		return curr;
	}
private:
	int i;
	int curr;
};

#endif
