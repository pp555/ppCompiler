#ifndef LOGICOPERATION_HH
#define LOGICOPERATION_HH

#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

extern TempIdentifier tempIdentifier;
extern Address addr;
extern ElementsStack elements;

namespace AstNodes
{
	class LogicOperation : public AstNode
	{
	public:
		
		enum Operator
		{
			Unnknown,
			And,
			Or
		};
		
		LogicOperation(Operator op, AstNode *lValue, AstNode *rValue) : AstNode(NodeType::LogicOperation), _lValue(lValue), _rValue(rValue), _operator(op)
		{}
		
		std::string codeGen()
		{
			//TODO
			std::cout << "TODO: logic operation " << _lValue->type() << '\t' << _rValue->type() << '\n';
			//valid types of values (left and right): BoolConstant, LogicOperation, Comparison
			
			switch(_operator)
			{
				case And:
					return codeForAnd();
					break;
				case Or:
					return codeForOr();
					break;
				default:
					std::cerr << "ERROR:\twrong logical operation\n";
					exit(1);
					return "";
			}
		}
		
		static LogicOperation *createFromStack(Operator op)
		{
			AstNode *rValue = elements.get();
			AstNode *lValue = elements.get();
			return new LogicOperation(op, lValue, rValue);
		}
		
	private:
		std::string codeForAnd()
		{
			
			labelsStack.push(labels.next());
			
			std::stringstream result;
			
			result << "andStart" << labelsStack.top() << ":";
			
			//condition 1:
			switch(_lValue->type())
			{
				case NodeType::BoolConstant:
					result << "ADD 0," << _lValue->codeGen() << ENDLINE
						<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
					 
					break;
				case NodeType::Comparison: //TODO
				case NodeType::LogicOperation:
					std::cout << "TODO: AstNodes::LogicOperation in lValue of op\n";
					exit(0); 
					break;
				default:
					std::cerr << "wrong node type in left side of logic operation\n";
					exit(1);
			}
			
			
			
			//condition 2:
			switch(_rValue->type())
			{
				case NodeType::BoolConstant:
					result << "ADD 0," << _rValue->codeGen() << ENDLINE
						<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
					 
					break;
				case NodeType::Comparison: //TODO
				case NodeType::LogicOperation:
					std::cout << "TODO: AstNodes::LogicOperation in lValue of op\n";
					exit(0); 
					break;
				default:
					std::cerr << "wrong node type in left side of logic operation\n";
					exit(1);
			}
			
			
			
			//result:
			addTempSymbol();
			
			//true:
			result << "andTrue" << labelsStack.top() << ":"
				<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",1" << ENDLINE
				<< "JMP " << "andEnd" << labelsStack.top() << ENDLINE;
			
			//false:
			result << "andFalse" << labelsStack.top() << ":"
				<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",0" << ENDLINE;
			
			result << "andEnd" << labelsStack.top() << ":";
			
			labelsStack.pop();
			
			return result.str();
		}
		
		std::string codeForOr()
		{
			return "";
		}
		
		void addTempSymbol()
		{
			symbols.insert(tempIdentifier.next(), TypeBool, addr.next());
			elements.add(new Variable(tempIdentifier.current(), TypeBool));
		}
		
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
}

#endif
