#ifndef ASSIGNMENT_HH
#define ASSIGNMENT_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

namespace AstNodes
{
	class Assignment : public AstNode
	{
	public:
		Assignment(AstNode *lValue, AstNode *rValue) : AstNode(NodeType::Assignment), _lValue(lValue), _rValue(rValue)
		{
		}
		
		std::string codeGen()
		{
			//lValue must be variable
			if(NodeType::Variable != _lValue->type())
			{
				std::cerr << "assignment left value must be variable\n";
				exit(1);
			}
			
			//check number types
			if(_lValue->numType() != _rValue->numType() || _lValue->numType() == None)
			{
				std::cerr << "type mismatch in assignment, conversion is not allowed\n";
				exit(1);
			}
			
			//rValue can be: Number, Variable, ArithmeticOperation
			if(NodeType::Number == _rValue->type() || NodeType::Variable == _rValue->type())
			{
				return "MOV " + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _rValue->type())
			{
				std::stringstream result;
				result << _rValue->codeGen();
				result << "MOV " + _lValue->codeGen() + "," + elements.get()->codeGen() + ENDLINE;
				return result.str();
			}
			else
			{
				std::cerr << "incorrect right side of assignment\n";
				exit(1);
			}
			
		}
		
	private:
		AstNode *_lValue;
		AstNode *_rValue;
	};
}

#endif
