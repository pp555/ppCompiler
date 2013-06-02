#ifndef PREPAREDCODE_HH
#define PREPAREDCODE_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

namespace AstNodes
{
	class PreparedCode : public AstNode
	{
	public:
		PreparedCode(std::string code, NumberType numType = None) : AstNode(NodeType::PreparedCode), _code(code), _numType(numType)
		{}
		
		std::string codeGen()
		{
			return _code;
		}
		
		NumberType numType() const
		{
			return _numType;
		}
		
	private:
		std::string _code;
		NumberType _numType;
	};
	
}
#endif
