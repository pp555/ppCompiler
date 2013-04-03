#ifndef TEMPIDENTIFIER_HH
#define TEMPIDENTIFIER_HH

#include <string>
#include <sstream>

class TempIdentifier
{
public:
	TempIdentifier() : i(-1)
	{}
	std::string next()
	{
		std::stringstream stream;
		stream << 't' << ++i;
		return stream.str();
	}
	std::string current() const
	{
		std::stringstream stream;
		stream << 't' << i;
		return stream.str();
	}

private:
	int i;
};

#endif
