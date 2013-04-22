#ifndef IFSTMT_HH
#define IFSTMT_HH

#include <string>
#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

extern TempIdentifier label;

namespace AstNodes
{
	class IfStmt : public AstNode
	{
	public:
		IfStmt(AstNode *codeBlock, AstNode *condition) : AstNode(NodeType::IfStmt), _codeBlock(codeBlock), _condition(condition)
		{
			//std::cout << "adding if stmt\n" << codeBlock->type() << "\t" << condition->type() << std::endl;
		}
		
		std::string codeGen()
		{
			label.next();
			std::stringstream result;
			if(NodeType::Number == _condition->type() || NodeType::Variable == _condition->type())
			{
				//condition:!=0
				result << "SUB " << _condition->codeGen()  << ",0" << ENDLINE;
				result << "JZ label.e" << label.current() << ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _condition->type())
			{
				result << _condition->codeGen();
				//condition:!=0
				result << "SUB " << elements.get()->codeGen() << ",0" << ENDLINE;
				result << "JZ label.e" << label.current() << ENDLINE;
			}
			else if(NodeType::Comparison == _condition->type())
			{
				result << _condition->codeGen() << "label.e" << label.current() << ENDLINE;
			}
			else
			{
				std::cerr << "incorrect if condition\n";
				exit(1);
			}
			
			result << "label.s" << label.current() << ":" << _codeBlock->codeGen() << "label.e" << label.current() << ":";
			return result.str();
		}
		
	private:
		AstNode *_codeBlock;
		AstNode *_condition;
	};
}

#endif