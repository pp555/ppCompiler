#ifndef BOOLCONSTANT_HH
#define BOOLCONSTANT_HH

#include <sstream>

#include "../enums.hh"

namespace AstNodes
{
	class BoolConstant : public AstNode
	{
	public:
		BoolConstant(bool value) : AstNode(NodeType::BoolConstant)
		{
			_val = value;
		}
		
		std::string codeGen()
		{
			std::stringstream stream;
			stream << (_val?1:0);
			return stream.str();
		}
		
	private:
		bool _val;
	};
}

#endif
