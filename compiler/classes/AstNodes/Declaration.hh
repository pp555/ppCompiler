#ifndef DECLARATION_HH
#define DECLARATION_HH

#include <string>
#include <sstream>

#include "../enums.hh"
#include "AstNode.hh"

extern SymbolsMap symbols;

namespace AstNodes
{
	class Declaration : public AstNode
	{
	public:
		Declaration(NumberType type, const std::string &name) : AstNode(NodeType::Declaration), _type(type), _name(name)
		{}
		
		
		Declaration(NumberType type, Variable *var) : AstNode(NodeType::Declaration), _type(type), _name(var->name())
		{
		}
		
		std::string codeGen()
		{
			symbols.insert(_name, _type, addr.next());
			return "";
		}
		
		std::string name() const {return _name;}
		NumberType numType() const {return _type;}
	private:
		std::string _name;
		NumberType _type;
	};
}

#endif
