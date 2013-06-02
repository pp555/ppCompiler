#ifndef ASTNODE_HH
#define ASTNODE_HH

#include <cstdlib>

#include "../enums.hh"

namespace AstNodes
{
	class AstNode
	{
	public:
	
		AstNode(NodeType::Type type) : _type(type)
		{}
		virtual ~AstNode()
		{}
		virtual std::string codeGen() = 0;
		
		NodeType::Type type()
		{
			return _type;
		}
		
		virtual std::string toString(){return "AstNode";};
		
		virtual NumberType numType() const
		{
			std::cerr << "not a number\tnode type:\t" << _type << "\n";
			exit(1);
		}
	private:
		NodeType::Type _type;
	};
}

#endif
