#ifndef VARIABLE_HH
#define VARIABLE_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

extern SymbolsMap symbols;

namespace AstNodes
{
	class Variable : public AstNode
	{
	public:
		Variable(const std::string &name, NumberType type = None) : AstNode(NodeType::Variable), _name(name), _numberType(type)
		{
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			result << "#" << symbols[_name].offset();
			_numberType = symbols[_name].type();
			return result.str();
		}

		NumberType numType() const
		{
			if(_numberType == None)
			{
				if(symbols.contains(_name))
				{
					return symbols[_name].type();
				}
				std::cerr << "none type detected while getting variable type (undeclared variable):\t" << _name << '\n';
				exit(1);
			}
			return _numberType;
		}
		
		std::string name() const
		{
			return _name;
		}
	private:
		std::string _name;
		NumberType _numberType;
	};
}

#endif
