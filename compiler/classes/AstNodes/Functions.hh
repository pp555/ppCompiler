#ifndef FUNCTIONS_HH
#define FUNCTIONS_HH

#include <string>
#include <sstream>
#include <cstdlib>
#include <list>

#include "../enums.hh"
#include "AstNode.hh"
#include "CodeBlock.hh"

extern SymbolsMap symbols;
extern Address labels;

class Function
{
public:
	Function(AstNodes::AstNode *codeBlock, AstNodes::AstNode *nVar)
		: _name(static_cast<AstNodes::Variable*>(nVar)->name()), _codeBlock(static_cast<AstNodes::CodeBlock*>(codeBlock))
	{
		std::cout << "function definition \t name: " << _name << "\n";
		std::cout << args.size() << "arguments\n";
		
		symbols.insert("$" + _name + ".%retValue", NumInt, addr.next());
		
		Function::args.clear();
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
	
public:
	static std::list<AstNodes::AstNode*> args;
	
private:
	std::string _name;
	AstNodes::CodeBlock *_codeBlock;
	
	NumberType _returnType;
	
};
std::list<AstNodes::AstNode*> Function::args;

std::vector<Function*> functions;


namespace AstNodes
{
	class FunctionCall : public AstNode
	{
	public:
		FunctionCall(std::string name)
			: AstNode(NodeType::FunctionCall), _name(name)
		{
			std::cout << "function call \t name: " << _name << "\n";
			std::cout << Function::args.size() << " arguments\n";
			
			Function::args.clear();
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			
			
			result << "PUSH label.afterfcall" << labels.next() << ENDLINE;
			result << "JMP function." << _name << ENDLINE;
			result << "label.afterfcall" << labels.current() << ":";
			
			return result.str();
		}
	
		NumberType numType() const
		{
			return NumInt;
		}
		
		std::string name() const
		{
			return _name;
		}
		
	private:
		std::string _name;
	};
	
}

#endif
