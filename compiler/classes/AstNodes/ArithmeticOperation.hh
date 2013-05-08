#ifndef ARITHMETICOPERATION_HH
#define ARITHMETICOPERATION_HH

#include <sstream>
#include <cstdlib>

#include "../enums.hh"
#include "AstNode.hh"

extern TempIdentifier tempIdentifier;
extern Address addr;
extern ElementsStack elements;

namespace AstNodes
{
	class ArithmeticOperation : public AstNode
	{
	public:
		enum Operator
		{
			Unnknown,
			Add,
			Sub,
			Mul,
			Div
		};
		
		ArithmeticOperation(const std::string &op, AstNode *lValue, AstNode *rValue)
			: AstNode(NodeType::ArithmeticOperation), _lValue(lValue), _rValue(rValue), _operator(Unnknown)
		{
			if("+" == op)
			{
				_operator = Add;
			}
			else if("-" == op)
			{
				_operator = Sub;
			}
			else if("*" == op)
			{
				_operator = Mul;
			}
			else if("/" == op)
			{
				_operator = Div;
			}
		}
		ArithmeticOperation(Operator op, AstNode *lValue, AstNode *rValue) : AstNode(NodeType::ArithmeticOperation), _lValue(lValue), _rValue(rValue), _operator(op)
		{}
		
		std::string codeGen()
		{
			std::stringstream result;

			NumberType lType = this->numType();
			
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
			
			addTempSymbol();
			
			switch(lType)
			{
				case NumInt:
					result << "MOV R1," << _lValue->codeGen() << ENDLINE
						<< "MOV R2," << _rValue->codeGen() << ENDLINE
						<< opAsmName() << " R1,R2" << ENDLINE
						<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",R1" << ENDLINE;
					break;
				case NumFloat:
					result << "FMOV F1," << _lValue->codeGen() << ENDLINE
						<< "FMOV F2," << _rValue->codeGen() << ENDLINE
						<< 'F' << opAsmName() << " F1,F2" << ENDLINE
						<< "FMOV #" << symbols[tempIdentifier.current()].offset() << ",F1" << ENDLINE;
					break;
				default:
					std::cerr << "none type in arithmetic operation\n";
					exit(1);
			}
			
			
			return result.str();
		}
		
		void addTempSymbol()
		{
			NumberType tempType = this->numType();
			symbols.insert(tempIdentifier.next(), tempType, addr.next());
			elements.add(new Variable(tempIdentifier.current(), tempType));
		}
		
		static ArithmeticOperation *createFromStack(const std::string &op)
		{
			AstNode *rValue = elements.get();
			AstNode *lValue = elements.get();
			return new AstNodes::ArithmeticOperation(op, lValue, rValue);
		}
		
		NumberType numType() const
		{
			NumberType lType = _lValue->numType();
			NumberType rType = _rValue->numType();
			if(lType == rType)
			{
				if(lType == None)
				{
					std::cerr << "none type detected while checking type in arithmetic operation\n";
					exit(1);
				}
				return lType;
			}
			else if(lType == NumFloat || rType == NumInt)
			{
				std::cout << "warning: type mismatch in operation\n";
				return NumFloat;
			}
			else
			{
				std::cerr << "type mismatch in operation, conversion not allowed\n";
				exit(1);
			}
		}
		
	private:
		std::string opAsmName()
		{
			switch(_operator)
			{
			case Add:
				return "ADD";
			case Sub:
				return "SUB";
			case Mul:
				return "MUL";
			case Div:
				return "DIV";
			default:
				std::cerr << "unnknown arithmetic operator" << std::endl;
				exit(1);
			}
		}
	
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
}


#endif
