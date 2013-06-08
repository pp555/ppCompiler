#ifndef AST_HH
#define AST_HH

#include <stack>

#include "Symbol.hh"
#include "SymbolsMap.hh"
#include "TempIdentifier.hh"
#include "Address.hh"
#include "enums.hh"
#include "ElementsStack.hh"

#define ENDLINE "\n"

#include "AstNodes/PreparedCode.hh"
#include "AstNodes/AstNode.hh"
#include "AstNodes/Number.hh"
#include "AstNodes/Variable.hh"
#include "AstNodes/ArithmeticOperation.hh"
#include "AstNodes/Assignment.hh"
#include "AstNodes/Declaration.hh"
#include "AstNodes/CodeBlock.hh"
#include "AstNodes/Comparison.hh"
#include "AstNodes/IfStmt.hh"
#include "AstNodes/Loops.hh"
#include "AstNodes/ComplexCondition.hh"
#include "AstNodes/BoolConstant.hh"
#include "AstNodes/LogicOperation.hh"
#include "AstNodes/Array.hh"

extern int yylineno;

SymbolsMap symbols;
TempIdentifier tempIdentifier("%t");
Address addr;
ElementsStack elements;
std::stack<AstNodes::CodeBlock *> blocksStack;
std::stack<int> labelsStack;
Address labels;

//TODO:
class Function
{
public:
	Function(AstNodes::AstNode *codeBlock, AstNodes::AstNode *nVar)
		: _name(static_cast<AstNodes::Variable*>(nVar)->name()), _codeBlock(static_cast<AstNodes::CodeBlock*>(codeBlock))
	{
		std::cout << "function definition \t name: " << _name << "\n";
	}
	
	std::string codeGen()
	{
		std::stringstream result;
		
		result << "function." << _name << ":"; 
		
		//add temp symbol name.%returnline 
		symbols.insert("$" + _name + ".%returnline", NumInt, addr.next());
		AstNodes::Variable *returnLineVar = new AstNodes::Variable("$" + _name + ".%returnline", NumInt);
		result << "POP " << returnLineVar->codeGen() << ENDLINE;
		
		result << _codeBlock->codeGen();
		result << "JMP " << returnLineVar->codeGen() << ENDLINE;
		
		return result.str();
	}
	
	
private:
	std::string _name;
	AstNodes::CodeBlock *_codeBlock;
	
	NumberType _returnType;
	
};
std::vector<Function*> functions;

namespace AstNodes
{
	class FunctionCall : public AstNode
	{
	public:
		FunctionCall(std::string name)
			: AstNode(NodeType::FunctionCall), _name(name)
		{}
		
		std::string codeGen()
		{
			std::stringstream result;
			
			
			result << "PUSH label.afterfcall" << labels.next() << ENDLINE;
			result << "JMP function." << _name << ENDLINE;
			result << "label.afterfcall" << labels.current() << ":";
			
			return result.str();
		}
	private:
		std::string _name;
	};
	
}

#endif //AST_HH
