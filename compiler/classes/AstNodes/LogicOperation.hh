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
				case NodeType::Comparison:
					result << _lValue->codeGen() << "andFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::LogicOperation:
					result << _lValue->codeGen();
					result << "ADD " << elements.get()->codeGen() << ",0" << ENDLINE
						<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Variable:
					switch(_lValue->numType())
					{
						case TypeBool:
						case NumInt:
							result << "SUB " << _lValue->codeGen()  << ",0" << ENDLINE
								<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
							break;
						default:
							std::cerr << "wrong variable type in logic operation";
							exit(1);
					}
					break;
				default:
					std::cerr << "wrong node type in left side of logic operation:" << _lValue->type() << "\n";
					exit(1);
			}
			
			//condition 2:
			switch(_rValue->type())
			{
				case NodeType::BoolConstant:
					result << "ADD 0," << _rValue->codeGen() << ENDLINE
						<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Comparison:
					result << _rValue->codeGen() << "andFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::LogicOperation:
					result << _rValue->codeGen();
					result << "ADD " << elements.get()->codeGen() << ",0" << ENDLINE
						<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Variable:
					switch(_rValue->numType())
					{
						case TypeBool:
						case NumInt:
							result << "SUB " << _rValue->codeGen()  << ",0" << ENDLINE
								<< "JZ " << "andFalse" << labelsStack.top() << ENDLINE;
							break;
						default:
							std::cerr << "wrong variable type in logic operation";
							exit(1);
					}
					break;
				default:
					std::cerr << "wrong node type in right side of logic operation:" << _rValue->type() << "\n";
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
			labelsStack.push(labels.next());
			
			std::stringstream result;
			
			result << "orStart" << labelsStack.top() << ":";
			
			//condition 1:
			switch(_lValue->type())
			{
				case NodeType::BoolConstant:
					result << "ADD 0," << _lValue->codeGen() << ENDLINE
						<< "JZ " << "orSecondCond" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Comparison:
					result << _lValue->codeGen() << "orSecondCond" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::LogicOperation:
					result << _lValue->codeGen();
					result << "ADD " << elements.get()->codeGen() << ",0" << ENDLINE
						<< "JZ " << "orSecondCond" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Variable:
					switch(_lValue->numType())
					{
						case TypeBool:
						case NumInt:
							result << "SUB " << _lValue->codeGen()  << ",0" << ENDLINE
								<< "JZ " << "orSecondCond" << labelsStack.top() << ENDLINE;
							break;
						default:
							std::cerr << "wrong variable type in logic operation";
							exit(1);
					}
					break;
				default:
					std::cerr << "wrong node type in left side of logic operation:" << _lValue->type() << "\n";
					exit(1);
			}
			result << "JMP " << "orTrue" << labelsStack.top() << ENDLINE
				<< "orSecondCond" << labelsStack.top() << ":";
			
			//condition 2:
			switch(_rValue->type())
			{
				case NodeType::BoolConstant:
					result << "ADD 0," << _rValue->codeGen() << ENDLINE
						<< "JZ " << "orFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Comparison:
					result << _rValue->codeGen() << "orFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::LogicOperation:
					result << _rValue->codeGen();
					result << "ADD " << elements.get()->codeGen() << ",0" << ENDLINE
						<< "JZ " << "orFalse" << labelsStack.top() << ENDLINE;
					break;
				case NodeType::Variable:
					switch(_rValue->numType())
					{
						case TypeBool:
						case NumInt:
							result << "SUB " << _rValue->codeGen()  << ",0" << ENDLINE
								<< "JZ " << "orFalse" << labelsStack.top() << ENDLINE;
							break;
						default:
							std::cerr << "wrong variable type in logic operation";
							exit(1);
					}
					break;
				default:
					std::cerr << "wrong node type in right side of logic operation:" << _rValue->type() << "\n";
					exit(1);
			}
			
			//result:
			addTempSymbol();
			
			//true:
			result << "orTrue" << labelsStack.top() << ":"
				<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",1" << ENDLINE
				<< "JMP " << "orEnd" << labelsStack.top() << ENDLINE;
			
			//false:
			result << "orFalse" << labelsStack.top() << ":"
				<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",0" << ENDLINE;
			
			result << "orEnd" << labelsStack.top() << ":";
			
			labelsStack.pop();
			
			return result.str();
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
