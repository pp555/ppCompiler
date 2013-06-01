#ifndef IFSTMT_HH
#define IFSTMT_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

extern Address labels;
extern std::stack<int> labelsStack;

namespace AstNodes
{
	class IfStmt : public AstNode
	{
	public:
		IfStmt(AstNode *codeBlock, AstNode *condition, AstNode *elseBlock = NULL) : AstNode(NodeType::IfStmt), _codeBlock(codeBlock), _condition(condition), _elseBlock(elseBlock)
		{
			//std::cout << "adding if stmt\n" << codeBlock->type() << "\t" << condition->type() << std::endl;
		}
		
		std::string codeGen()
		{
			labelsStack.push(labels.next());
			
			std::stringstream result;
			
			if(NodeType::Number == _condition->type()
				|| NodeType::Variable == _condition->type()
				|| NodeType::BoolConstant == _condition->type())
			{
				if(NodeType::BoolConstant != _condition->type() && _condition->numType() == NumFloat) {std::cerr << "cannot use float as condition\n";exit(1);}
				//condition:!=0
				result << "SUB " << _condition->codeGen()  << ",0" << ENDLINE;
				result << "JZ label.e" << labelsStack.top() << ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _condition->type())
			{
				result << _condition->codeGen();
				//condition:!=0
				result << "SUB " << elements.get()->codeGen() << ",0" << ENDLINE;
				result << "JZ label.e" << labelsStack.top() << ENDLINE;
			}
			else if(NodeType::Comparison == _condition->type())
			{
				result << _condition->codeGen() << "label.e" << labelsStack.top() << ENDLINE;
			}
			else if(NodeType::LogicOperation == _condition->type())
			{
				result << _condition->codeGen();
				result << "SUB " << elements.get()->codeGen() << ",0" << ENDLINE;
				result << "JZ label.e" << labelsStack.top() << ENDLINE;
			}
			
			/*
			else if(NodeType::ComplexCondition == _condition->type())
			{
				result << _condition->codeGen();
			}
			*/
			else
			{
				std::cerr << "incorrect if condition\n";
				exit(1);
			}
			
			result << "label.s" << labelsStack.top() << ":" << _codeBlock->codeGen();
			if(_elseBlock)
			{
				result << "JMP label.end" << labelsStack.top() << ENDLINE;
			}
			result << "label.e" << labelsStack.top() << ":";
			
			if(_elseBlock)
			{
				result << _elseBlock->codeGen() << "label.end" << labelsStack.top() << ":";
			}
			
			labelsStack.pop();
			
			return result.str();
		}
		
	private:
		AstNode *_codeBlock;
		AstNode *_condition;
		AstNode *_elseBlock;
	};
}

#endif
