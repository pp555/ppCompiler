#ifndef COMPLEXCONDITION_HH
#define COMPLEXCONDITION_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

namespace AstNodes
{
	class ComplexCondition : public AstNode
	{
	public:
		enum LogicOperator
		{
			Unnknown,
			And,
			Or
		};
		
		ComplexCondition(LogicOperator op, AstNode *lValue, AstNode *rValue) : _lValue(lValue), _rValue(rValue), _operator(op), AstNode(NodeType::ComplexCondition)
		{
		}
		
		std::string codeGen()
		{
			if((_lValue->type() != NodeType::ComplexCondition && _lValue->type() != NodeType::Comparison)
				|| (_rValue->type() != NodeType::ComplexCondition && _rValue->type() != NodeType::Comparison))
			{
				std::cerr << "complex condition elements must be other complex conditions or comparisons\n";
				exit(1);
			}
			
			std::stringstream result;
			
			switch(_operator)
			{
				case And:
					result << genCodeForAnd();
					break;
				case Or:
					result << genCodeForOr();
					break;
				default:
					std::cerr << "unnknown operator in complex condition\n";
					exit(1);
			}
			
			//TODO
			return result.str();
		}
	
	private:
		AstNode *_lValue;
		AstNode *_rValue;
		LogicOperator _operator;
		
		std::string genCodeForAnd()
		{
			std::stringstream result;
			//gen condition 1 code
			result << _lValue->codeGen();
			result << "label.e" << label.current();//TODO: is label number ok?
			result << ENDLINE;
			//gen condition 2 code
			result << _rValue->codeGen();
			result << "label.e" << label.current() << ENDLINE;
			
			return result.str();
		}
		
		std::string genCodeForOr()
		{
			//TODO
			std::stringstream result;
			//gen condition 1 code
			result << _lValue->codeGen();
			result << "label.2c" << label.current() << ENDLINE;//TODO: is label number ok?
			result << "JMP " << "label.s" << label.current() << ENDLINE;
			//gen condition 2 code
			result << "label.2c" << label.current() << ":" << _rValue->codeGen();
			result << "label.e" << label.current() << ENDLINE;
			return result.str();
		}
		
	};
	
}

#endif
