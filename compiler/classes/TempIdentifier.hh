#ifndef TEMPIDENTIFIER_HH
#define TEMPIDENTIFIER_HH

#include <string>
#include <sstream>

class TempIdentifier
{
public:
	TempIdentifier(const std::string &prefix) : i(-1), _prefix(prefix)
	{}
	std::string next()
	{
		std::stringstream stream;
		stream << _prefix << ++i;
		return stream.str();
	}
	std::string current() const
	{
		std::stringstream stream;
		stream << _prefix << i;
		return stream.str();
	}

private:
	int i;
	std::string _prefix;
};

#endif
