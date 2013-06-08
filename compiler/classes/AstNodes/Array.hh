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
		Indexer(AstNode *n) : AstNode(NodeType::Indexer), _value(-1), _valueNode(n)
		{}
		int get() const
		{
			if(_value < 0)
			{
				std::cerr << "index cannot be lower than 0\n";
				exit(1);
			}
			return _value;
		}
		
		std::string codeGen()
		{
			if(_value == -1)
			{
				return _valueNode->codeGen();
			}
			std::stringstream vS;
			vS << _value;
			return vS.str();
		}
	private:
		int _value;
		AstNode *_valueNode;
		
	};
	
	class ArrayDeclaration : public AstNode
	{
	public:
		ArrayDeclaration(NumberType type) : AstNode(NodeType::ArrayDeclaration), _type(type)
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
	
	class ArrayVariable : public AstNode
	{
	public:
		ArrayVariable(NumberType type = None) : AstNode(NodeType::ArrayVariable), _numberType(type), _name("")
		{
			std::cout << "ArrayVariable c'tor\n";
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
					_indexes.push_back(static_cast<AstNodes::Indexer*>(indexerOrName));
				}
				else
				{
					std::cerr << "Expected indexer or variable name for array\n";
					exit(1);
				}
			}
			
			std::cout << "array variable:\t" << _name << "\t" << _indexes.size() << "\n";
			
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			
			if(!symbols[_name].isArray())
			{
				std::cerr << "indexes used with non-array variable\tname: " << _name << "\n";
				exit(1);
			}
			
			result << "MOV R7," << symbols[_name].offset() << ENDLINE;
			std::vector<int> size = symbols[_name].size();
			
			//use size, _indexes to get element offset
			int dim = size.size();
			
			for(int i=0;i<dim;i++)
				result << "MOV R" << i << "," << _indexes[i]->codeGen() << ENDLINE;
			for(int i=1;i<dim;i++)
			{
				int s=size[i-1];
				int j=i-1;
				while(--j>=0)
				{
					s*=size[j];
				}
				result << "MUL R" << i << "," << s << ENDLINE;
			}
			
			for(int i=0;i<dim;i++)
				result << "ADD R7,R" << i << ENDLINE;
			
			return result.str();
		}

		NumberType numType() const
		{
			if(_numberType == None)
			{
				if(symbols.contains(_name))
				{
					return symbols[_name].type();
				}
				std::cerr << "none type detected while getting variable type (undeclared variable)";
				exit(1);
			}
			return _numberType;
		}
		
		std::string name() const
		{
			return _name;
		}
	private:
		std::string _name;
		NumberType _numberType;
		std::vector<AstNodes::Indexer*> _indexes;
	};
	
	
}

#endif
