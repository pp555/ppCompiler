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
			
			return "";
		}
		
		void addTempSymbol()
		{
			NumberType tempType = TypeBool;
			symbols.insert(tempIdentifier.next(), tempType, addr.next());
			elements.add(new Variable(tempIdentifier.current(), tempType));
		}
		
		static LogicOperation *createFromStack(Operator op)
		{
			AstNode *rValue = elements.get();
			AstNode *lValue = elements.get();
			return new LogicOperation(op, lValue, rValue);
		}
		
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
}

#endif
