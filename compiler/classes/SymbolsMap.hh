#ifndef SYMBOLSMAP_HH
#define SYMBOLSMAP_HH

#include <map>
#include <cstdlib>

#include "enums.hh"

class SymbolsMap
{
public:
	SymbolsMap()
	{
	}

	void insert(const std::string &name, NumberType type, int address)
	{
		std::pair<std::map<std::string, Symbol>::iterator,bool> result = _symbols.insert(std::pair<std::string, Symbol>(name, Symbol(type, address)));
		
		if(!result.second)//already existed
		{
			std::cerr << "symbol redefinition:\t" << name << std::endl;
			exit(1);
		}
	}
	
	void insertArray(const std::string &name, NumberType type, int address, const std::vector<int> &size)
	{
		std::pair<std::map<std::string, Symbol>::iterator,bool> result = _symbols.insert(std::pair<std::string, Symbol>(name, Symbol(type, address, size)));
		
		if(!result.second)//already existed
		{
			std::cerr << "symbol redefinition:\t" << name << std::endl;
			exit(1);
		}
	}
	
	Symbol operator[](const std::string &name)
	{
		if(_symbols.find(name) == _symbols.end())
		{
			std::cerr << "undefined symbol:\t" << name << std::endl;
			exit(1);
		}
		return _symbols[name];
	}
	
	bool contains(const std::string &name) const
	{
		return _symbols.end() != _symbols.find(name);
	}
	
	void printSymbols()
	{
		std::cout << "ident\tadres\ttype\tisArr" << std::endl;
		for(std::map<std::string, Symbol>::iterator it = _symbols.begin(); it != _symbols.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second.offset() << "\t" << it->second.type() << "\t" << it->second.isArray() << std::endl;
		}

	}
	
	

private:
	std::map<std::string, Symbol> _symbols;
};

#endif
