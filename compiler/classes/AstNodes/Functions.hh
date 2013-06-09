#ifndef FUNCTIONS_HH
#define FUNCTIONS_HH

#include <string>
#include <sstream>
#include <cstdlib>
#include <list>

#include "../enums.hh"
#include "AstNode.hh"
#include "CodeBlock.hh"
#include "Declaration.hh"

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
		
		for(int i=0;i<args.size();++i)
		{
			arguments.push_back(static_cast<AstNodes::Declaration*>(args.at(i)));
		}
		
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
		
		//pop arguments
		for(int i=0;i<arguments.size();++i)
		{
			result << "POP #" << symbols[arguments.at(i)->name()].offset() << ENDLINE;
		}
		
		
		result << _codeBlock->codeGen();
		result << "JMP " << returnLineVar->codeGen() << ENDLINE;
		
		return result.str();
	}
	
	void initSymbols()
	{
		for(int i=0;i<arguments.size();++i)
		{
			symbols.insert(arguments.at(i)->name(), arguments.at(i)->numType(), addr.next());
		}
	}
	void deinitSymbols()
	{
		for(int i=0;i<arguments.size();++i)
		{
			symbols.remove(arguments.at(i)->name());
		}
	}
	
private:
	std::string _name;
	AstNodes::CodeBlock *_codeBlock;
	std::vector<AstNodes::Declaration*> arguments; //vector of Declaration objects
	NumberType _returnType;
	
public:
	static std::vector<AstNodes::AstNode*> args;
};
std::vector<AstNodes::AstNode*> Function::args;

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
			
			for(int i=0;i<Function::args.size();++i)
			{
				arguments.push_back(static_cast<AstNodes::Variable*>(Function::args.at(i)));
			}
		
			Function::args.clear();
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			
			
			for(int i=0;i<arguments.size();++i)
			{
				result << "PUSH " << arguments.at(i)->codeGen() << ENDLINE;
			}
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
		std::vector<AstNodes::Variable*> arguments;
	};
	
}

#endif
