#ifndef COMPARISON_HH
#define COMPARISON_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

namespace AstNodes
{
	class Comparison : public AstNode
	{
	public:
		enum Operator
		{
			Unknown,
			lt,
			gt,
			le,
			ge,
			eq,
			ne
		};
		Comparison(const std::string &op, AstNode *lValue, AstNode *rValue)
			: AstNode(NodeType::Comparison), _operator(Unknown), _lValue(lValue), _rValue(rValue)
		{
			if("<" == op)
				_operator = lt;
			else if(">" == op)
				_operator = gt;
			else if("<=" == op)
				_operator = le;
			else if(">=" == op)
				_operator = ge;
			else if("==" == op)
				_operator = eq;
			else if("!=" == op)
				_operator = ne;
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			
			//expand lvalue
			if(NodeType::Number == _lValue->type() || NodeType::Variable == _lValue->type())
			{
			}
			else if(NodeType::ArithmeticOperation == _lValue->type())
			{
				result << _lValue->codeGen();
				_lValue = elements.get();
			}
			else
			{
				std::cerr << "incorrect node type\n";
			}
			//expand rvalue
			if(NodeType::Number == _rValue->type() || NodeType::Variable == _rValue->type())
			{
			}
			else if(NodeType::ArithmeticOperation == _rValue->type())
			{
				result << _rValue->codeGen();
				_rValue = elements.get();
			}
			else
			{
				std::cerr << "incorrect node type\n";
			}
			
			NumberType lType = _lValue->numType();
			if(lType != _rValue->numType())
			{
				if(lType == NumFloat || _rValue->numType() == NumFloat)
				{
					lType = NumFloat;
				}
				else
				{
					std::cerr << "type mismatch in comparison\n";
					exit(1);
				}
			}
			
			switch(lType)
			{
				case NumInt:
					result << "MOV R1," << _lValue->codeGen() << ENDLINE
						   << "MOV R2," << _rValue->codeGen() << ENDLINE
						   << "SUB R1,R2" << ENDLINE
						   << opAsmName() << ' ';
					break;
				case NumFloat:
					result << "FMOV F1," << _lValue->codeGen() << ENDLINE
						   << "FMOV F2," << _rValue->codeGen() << ENDLINE
						   << "FSUB F1,F2" << ENDLINE
						   << opAsmName() << ' ';
					break;
				default:
					std::cerr << "none type in comparison\n";
					exit(1);
			}
			
			return result.str();
		}
	private:
		std::string opAsmName()
		{
			switch(_operator)
			{
			case lt: return "JNL";
			case gt: return "JNG";
			case le: return "JG";
			case ge: return "JL";
			case eq: return "JNZ";
			case ne: return "JZ";
			default:
				std::cerr << "unnknown comparison operator\n";
				exit(1);
			}
		}
	
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
}

#endif
