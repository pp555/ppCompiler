#ifndef ASSIGNMENT_HH
#define ASSIGNMENT_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"
#include "Array.hh"

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
			std::stringstream result;
			//lValue must be variable
			if(NodeType::Variable != _lValue->type())
			{
				if(NodeType::ArrayVariable == _lValue->type())
				{
					std::cout << "TODO:arr var in assgn\n";
					
					//ArrayVariable *l = static_cast<ArrayVariable*>(_lValue);
					//std::cout << "TODO\n";
					//std::cout << l->name() << "\n";
					
					result << _lValue->codeGen();
					_lValue = new PreparedCode("@R7", _lValue->numType());
					
				}
				else
				{
					std::cerr << "assignment left value must be variable\n";
					exit(1);
				}
			}
			
			if(NodeType::BoolConstant == _rValue->type() || NodeType::LogicOperation == _rValue->type())//bool type assignment
			{
				if(_lValue->numType() != TypeBool) {std::cerr << "tried to assign bool to non-bool variable\n";exit(1);}
				
				if(NodeType::BoolConstant == _rValue->type())
				{
					result << "MOV " + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
					return result.str();
				}
					
				//NodeType::LogicOperation == _rValue->type()
				result << _rValue->codeGen();
				result << "MOV" << ' ' + _lValue->codeGen() + "," + elements.get()->codeGen() + ENDLINE;
				return result.str();
			}
			//otherwise its number type assignment
			
			//check number types
			NumberType lType = _lValue->numType();
			if(lType != _rValue->numType())
			{
				if(lType == None)
				{
					std::cerr << "not a number (type: None)\n";
					exit(1);
				}
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
				case TypeBool:
					movAsmCmd = "MOV";
					break;
				default:
					std::cerr << "none type in assignment\n";
					exit(1);
			}
			
			//rValue can be: Number, Variable, ArithmeticOperation
			if(NodeType::Number == _rValue->type() || NodeType::Variable == _rValue->type())
			{
				result << movAsmCmd + ' ' + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
				return result.str();
			}
			else if(NodeType::ArithmeticOperation == _rValue->type())
			{
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
