#ifndef AST_HH
#define AST_HH

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>
#include <stack>
#include <list>

#include "TempIdentifier.hh"
#include "Address.hh"

#define ENDLINE "\n"

typedef std::string Code;//TODO: vectorof strings

extern int yylineno;//TODO

enum NumberType
{
	NumInt,
	NumFloat
};

class SymbolsMap
{
public:
	SymbolsMap()
	{
	}

	void insert(const std::string &name, int address)
	{
		std::pair<std::map<std::string, int>::iterator,bool> result = _symbols.insert(std::pair<std::string, int>(name, address));
		
		if(!result.second)//already existed
		{
			std::cerr << "line:" << yylineno << ":\tsymbol redefinition:\t" << name << std::endl;
			exit(1);
		}
	}
	
	int operator[](const std::string &name)
	{
		if(_symbols.find(name) == _symbols.end())
		{
			std::cerr << "line:" << yylineno << ":\tundefined symbol:\t" << name << std::endl;
			exit(1);
		}
		return _symbols[name];
	}
	void printSymbols()
	{
		std::cout << "ident\tadres" << std::endl;
		for(std::map<std::string, int>::iterator it = _symbols.begin(); it != _symbols.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second << std::endl;
		}
	}
	
	

private:
	std::map<std::string, int> _symbols;
};


SymbolsMap symbols;

TempIdentifier tempIdentifier;
Address addr;

class NodeType
{
public:
	enum Type
	{
		Number = 0,
		Variable = 1,
		ArithmeticOperation = 2,
		Assignment = 3,
		Declaration = 4,
		CodeBlock = 5,
		IfStmt = 6
	};
};
		
namespace AstNodes
{
		
	class AstNode
	{
	public:
	
		AstNode(NodeType::Type type) : _type(type)
		{}
		virtual ~AstNode()
		{}
		virtual Code codeGen() = 0;
		
		NodeType::Type type()
		{
			return _type;
		}
		
		virtual std::string toString(){return "AstNode";};
		
	private:
		NodeType::Type _type;
	};
}
	

class ElementsStack
{
public:
	ElementsStack()
	{}
	
	AstNodes::AstNode *get()
	{
		AstNodes::AstNode *node = _elements.top();
		_elements.pop();
		return node;
	}
	
	void add(AstNodes::AstNode *node)
	{
		_elements.push(node);
	}
	
	bool isEmpty()
	{
		return _elements.empty();
	}
private:
	std::stack<AstNodes::AstNode*> _elements;//TODO
	
};

ElementsStack elements;

namespace AstNodes
{
	
	class Number : public AstNode
	{
	public:
		Number(double value) : AstNode(NodeType::Number), _numberType(NumFloat)
		{
			_val.fval = value;
		}
		Number(float value) : AstNode(NodeType::Number), _numberType(NumFloat)
		{
			_val.fval = value;
		}
		Number(int value) : AstNode(NodeType::Number), _numberType(NumInt)
		{
			_val.ival = value;
		}
		
		Code codeGen()
		{
			std::stringstream stream;
			switch(_numberType)
			{
			case NumInt:
				stream << _val.ival;
				break;
			case NumFloat:
				stream << _val.fval;
				break;
			}

			return stream.str();
		}
		
	private:
		
		union
		{
			int ival;
			float fval;
		} _val;

		NumberType _numberType;
	};
	
	
	class Variable : public AstNode
	{
	public:
		Variable(const std::string &name) : AstNode(NodeType::Variable), _name(name)
		{
			
		}
		
		Code codeGen()
		{
			std::stringstream result;
			result << "#" << symbols[_name];
			return result.str();;
		}
		
		std::string name() const
		{
			return _name;
		}
	private:
		std::string _name;
	};
	
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
		
		ArithmeticOperation(const std::string &op, AstNode *lValue, AstNode *rValue) : AstNode(NodeType::ArithmeticOperation), _lValue(lValue), _rValue(rValue), _operator(Unnknown)
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
		
		Code codeGen()
		{
			std::stringstream result;
			
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
				std::cerr << "line:" << yylineno << ":\tincorrect node type\n";
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
				std::cerr << "line:" << yylineno << ":\tincorrect node type\n";
			}
			
			
			addTempSymbol();
			result << "MOV R1," << _lValue->codeGen() << std::endl
				<< "MOV R2," << _rValue->codeGen() << std::endl
				<< opAsmName() << " R1,R2" << std::endl
				<< "MOV #" << symbols[tempIdentifier.current()] << ",R1" << std::endl;
			
			return result.str();
		}
		
		void addTempSymbol()
		{
			symbols.insert(tempIdentifier.next(), addr.next());//TODO: add type
			elements.add(new Variable(tempIdentifier.current()));
		}
		
		static ArithmeticOperation *createFromStack(const std::string &op)
		{
			AstNode *rValue = elements.get();
			AstNode *lValue = elements.get();
			return new AstNodes::ArithmeticOperation(op, lValue, rValue);
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
				std::cerr << "line:" << yylineno << ":\tunnknown arithmetic operator" << std::endl;
				exit(1);
			}
		}
	
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
	
	
	class Assignment : public AstNode
	{
	public:
		Assignment(AstNode *lValue, AstNode *rValue) : AstNode(NodeType::Assignment), _lValue(lValue), _rValue(rValue)
		{
		}
		
		Code codeGen()
		{
			//lValue must be variable
			if(NodeType::Variable != _lValue->type())
			{
				std::cerr << "line:" << yylineno << ":\tassignment left value must be variable\n";
				exit(1);
			}
			
			//rValue can be: Number, Variable, ArithmeticOperation
			if(NodeType::Number == _rValue->type() || NodeType::Variable == _rValue->type())
			{
				return "MOV " + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _rValue->type())
			{
				std::stringstream result;
				result << _rValue->codeGen();
				result << "MOV " + _lValue->codeGen() + "," + elements.get()->codeGen() + ENDLINE;
				return result.str();
			}
			else
			{
				std::cerr << "line:" << yylineno << ":\tincorrect right side of assignment\n";
				exit(1);
			}
			
		}
		
	private:
		AstNode *_lValue;
		AstNode *_rValue;
	};
	
	class Declaration : public AstNode
	{
	public:
		Declaration(NumberType type, const std::string &name) : AstNode(NodeType::Declaration), _type(type), _name(name)
		{}
		
		
		Declaration(NumberType type, Variable *var) : AstNode(NodeType::Declaration), _type(type), _name(var->name())
		{
		}
		
		Code codeGen()
		{
			symbols.insert(_name, addr.next());
			return "";
		}
	private:
		std::string _name;
		NumberType _type;
	};
	
	
	class CodeBlock : public AstNode
	{
	public:
		CodeBlock() : AstNode(NodeType::CodeBlock)
		{
			/*
			std::cout << "new code block in line " << yylineno << std::endl;
			while(!elements.isEmpty())
			{
				AstNodes::AstNode *e = elements.get();
				_blockContent.push_front(e);
				std::cout << "added to block:\t" << e->type() << std::endl;
			}
			*/
		}
		
		Code codeGen()
		{
			std::cout << "gen code for block\n";
			
			std::stringstream result;
			
			for(std::list<AstNode*>::iterator it = _blockContent.begin(); it != _blockContent.end(); ++it)
			{
				result << (*it)->codeGen();
			}
			
			return result.str();
		}
		
		virtual std::string toString(){return "CodeBlock";};
		
		void addStmt(AstNode *node)
		{
			_blockContent.push_back(node);
		}
		
		int stmtCount() const
		{
			return _blockContent.size();
		}
		
	private:
		std::list<AstNode*> _blockContent;
		
	};
	
	class IfStmt : public AstNode
	{
	public:
		IfStmt(AstNode *codeBlock, AstNode *condition) : AstNode(NodeType::IfStmt), _codeBlock(codeBlock), _condition(condition)
		{
			//std::cout << "adding if stmt\n" << codeBlock->type() << "\t" << condition->type() << std::endl;
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			if(NodeType::Number == _condition->type() || NodeType::Variable == _condition->type())
			{
				//condition:!=0
				result << "SUB " << _condition->codeGen()  << ",0" << ENDLINE;
				result << "JZ label.e1" << ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _condition->type())
			{
				result << _condition->codeGen();
				//condition:!=0
				result << "SUB " << elements.get()->codeGen() << ",0" << ENDLINE;
				result << "JZ label.e1" << ENDLINE;
			}
			else//TODO: add logical expresions
			{
				std::cerr << "incorrect if condition\n";
				exit(1);
			}
			
			result << "label.s1:" << _codeBlock->codeGen() << "label.e1:";
			return result.str();
		}
		
	private:
		AstNode *_codeBlock;
		AstNode *_condition;
	};
}

std::stack<AstNodes::CodeBlock *> blocksStack;

#endif //AST_HH