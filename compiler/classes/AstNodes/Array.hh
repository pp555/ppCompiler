#ifndef ARRAY_HH
#define ARRAY_HH

#include <string>
#include <sstream>

#include "../enums.hh"
#include "AstNode.hh"

extern SymbolsMap symbols;

namespace AstNodes
{
	class Indexer: public AstNode
	{
	public:
		Indexer(int value) : AstNode(NodeType::Indexer), _value(value)
		{}
		std::string codeGen() {return "";}
		int get() const
		{
			return _value;
		}
	private:
		int _value;
		
	};
	
	class ArrayDeclaration : public AstNode
	{
	public:
		ArrayDeclaration(NumberType type/*, Variable *var*/) : AstNode(NodeType::ArrayDeclaration), _type(type)/*, _name(var->name())*/
		{
			std::cout << "ArrayDeclaration c'tor\n";
			
			while(true)
			{
				AstNode *indexerOrName = elements.get();
				if(indexerOrName->type() == NodeType::Variable)
				{
					_name = static_cast<AstNodes::Variable*>(indexerOrName)->name();
					std::cout << "indexers list end; array name:\t" << _name << "\n";
					break;
				}
				if(indexerOrName->type() == NodeType::Indexer)
				{
					std::cout << "indexer found\n";
					_size.push_back(static_cast<AstNodes::Indexer*>(indexerOrName)->get());
				}
				else
				{
					std::cerr << "Expected indexer or variable name for array\n";
					exit(1);
				}
			}
			
		}
		
		std::string codeGen()
		{
			int mem = 1;
			for(int i=0;i<_size.size();i++)
				mem *= _size.at(i);
			symbols.insertArray(_name, _type, addr.next(mem), _size);
			return "";
		}
	private:
		std::string _name;
		NumberType _type;
		std::vector<int> _size;
	};
	
	
}

#endif
