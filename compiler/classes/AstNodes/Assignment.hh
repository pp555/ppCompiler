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
			
			if(NodeType::BoolConstant == _rValue->type())//bool type assignment
			{
				if(_lValue->numType() != TypeBool) {std::cerr << "tried to assign bool to non-bool variable\n";exit(1);}
				
				return "MOV " + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
			}
			//otherwise its number type assignment
			
			//check number types
			NumberType lType = _lValue->numType();
			if(lType != _rValue->numType())
			{
				if(lType == NumFloat)
				{
					std::cout << "warning: type mismatch in assignment\n";
					lType = NumFloat;
				}
				else
				{
					std::cerr << "type mismatch in assignment, conversion is not allowed\n";
					exit(1);
				}
			}
			
			std::string movAsmCmd;
			switch(lType)
			{
				case NumInt:
					movAsmCmd = "MOV";
					break;
				case NumFloat:
					movAsmCmd = "FMOV";
					break;
				default:
					std::cerr << "none type in assignment\n";
					exit(1);
			}
			
			//rValue can be: Number, Variable, ArithmeticOperation
			if(NodeType::Number == _rValue->type() || NodeType::Variable == _rValue->type())
			{
				return movAsmCmd + ' ' + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _rValue->type())
			{
				std::stringstream result;
				result << _rValue->codeGen();
				result << movAsmCmd << ' ' + _lValue->codeGen() + "," + elements.get()->codeGen() + ENDLINE;
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
