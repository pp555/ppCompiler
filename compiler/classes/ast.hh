#ifndef AST_HH
#define AST_HH

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>
#include <stack>
#include <list>
#include <vector>

#include "TempIdentifier.hh"
#include "Address.hh"

#define ENDLINE "\n"

extern int yylineno;

enum NumberType
{
	None,
	NumInt,
	NumFloat
};

class Symbol
{
public:
	Symbol() : _type(None), _offset(-1)
	{}
	Symbol(NumberType type, int offset) : _type(type), _offset(offset)
	{}
//TODO: add constructor for arrays

	int offset() const {return _offset;}
	NumberType type() const
	{
		if(_type == None)
		{
			std::cerr << "none type detected while getting type of symbol";
			exit(1);
		}
		return _type;
	}

	bool isArray()
	{
		return _size.size() > 0;
	}

private:
	int _offset;
	NumberType _type;
	std::vector<int> _size;
};

class SymbolsMap
{
public:
	SymbolsMap()
	{
	}

	void insert(const std::string &name, NumberType type, int address)
	{
		std::pair<std::map<std::string, Symbol>::iterator,bool> result = _symbols.insert(std::pair<std::string, Symbol>(name, Symbol(type, address)));
		
		if(!result.second)//already existed
		{
			std::cerr << "line:" << yylineno << ":\tsymbol redefinition:\t" << name << std::endl;
			exit(1);
		}
	}
	
	Symbol operator[](const std::string &name)
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
		for(std::map<std::string, Symbol>::iterator it = _symbols.begin(); it != _symbols.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second.offset() << std::endl;
		}

	}
	
	

private:
	std::map<std::string, Symbol> _symbols;
};


SymbolsMap symbols;

TempIdentifier tempIdentifier("t");
Address addr;

class NodeType
{
public:
	enum Type
	{
		Number,				//0
		Variable,			//1
		ArithmeticOperation,//2
		Assignment,			//3
		Declaration,		//4
		CodeBlock,			//5
		IfStmt,				//6
		Comparison			//7
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
		virtual std::string codeGen() = 0;
		
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
	std::stack<AstNodes::AstNode*> _elements;
	
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
		
		std::string codeGen()
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

		NumberType numType() const {return _numberType;}
		
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
		Variable(const std::string &name) : AstNode(NodeType::Variable), _name(name), _numberType(None)
		{
			
		}
		
		std::string codeGen()
		{
			std::stringstream result;
			result << "#" << symbols[_name].offset();
			_numberType = symbols[_name].type();
			return result.str();
		}

		NumberType numType() const
		{
			if(_numberType == None)
			{
				std::cerr << "none type detected while getting variable type";
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

			NumberType lType;//=lValue()->numType()
			NumberType rType;

			
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
				<< "MOV #" << symbols[tempIdentifier.current()].offset() << ",R1" << std::endl; //TODO: type
			
			return result.str();
		}
		
		void addTempSymbol()
		{
			symbols.insert(tempIdentifier.next(), NumInt, addr.next());//TODO: add FLOAT type
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
		
		std::string codeGen()
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
		
		std::string codeGen()
		{
			symbols.insert(_name, _type, addr.next());
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
		
		std::string codeGen()
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
	
	class Comparison : public AstNode
	{
	public:
		enum Operator
		{
			Unknown,
			lt,
			gt,
			le,
			ge,
			eq,
			ne
		};
		Comparison(const std::string &op, AstNode *lValue, AstNode *rValue)
			: AstNode(NodeType::Comparison), _operator(Unknown), _lValue(lValue), _rValue(rValue)
		{
			if("<" == op)
				_operator = lt;
			else if(">" == op)
				_operator = gt;
			else if("<=" == op)
				_operator = le;
			else if(">=" == op)
				_operator = ge;
			else if("==" == op)
				_operator = eq;
			else if("!=" == op)
				_operator = ne;
		}
		
		std::string codeGen()
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
			
			result << "MOV R1," << _lValue->codeGen() << ENDLINE;
			result << "MOV R2," << _rValue->codeGen() << ENDLINE;
			
			
			result << "SUB R1,R2" << ENDLINE
					<< opAsmName() << ' ';
			
			return result.str();
		}
	private:
		std::string opAsmName()
		{
			switch(_operator)
			{
			case lt: return "JNL";
			case gt: return "JNG";
			case le: return "JG";
			case ge: return "JL";
			case eq: return "JNZ";
			case ne: return "JZ";
			default:
				std::cerr << "unnknown comparison operator\n";
				exit(1);
			}
		}
	
		Operator _operator;
		AstNode *_lValue;
		AstNode *_rValue;
	};
	
	TempIdentifier label("");
	class IfStmt : public AstNode
	{
	public:
		IfStmt(AstNode *codeBlock, AstNode *condition) : AstNode(NodeType::IfStmt), _codeBlock(codeBlock), _condition(condition)
		{
			//std::cout << "adding if stmt\n" << codeBlock->type() << "\t" << condition->type() << std::endl;
		}
		
		std::string codeGen()
		{
			label.next();
			std::stringstream result;
			if(NodeType::Number == _condition->type() || NodeType::Variable == _condition->type())
			{
				//condition:!=0
				result << "SUB " << _condition->codeGen()  << ",0" << ENDLINE;
				result << "JZ label.e" << label.current() << ENDLINE;
			}
			else if(NodeType::ArithmeticOperation == _condition->type())
			{
				result << _condition->codeGen();
				//condition:!=0
				result << "SUB " << elements.get()->codeGen() << ",0" << ENDLINE;
				result << "JZ label.e" << label.current() << ENDLINE;
			}
			else if(NodeType::Comparison == _condition->type())
			{
				result << _condition->codeGen() << "label.e" << label.current() << ENDLINE;
			}
			else
			{
				std::cerr << "incorrect if condition\n";
				exit(1);
			}
			
			result << "label.s" << label.current() << ":" << _codeBlock->codeGen() << "label.e" << label.current() << ":";
			return result.str();
		}
		
	private:
		AstNode *_codeBlock;
		AstNode *_condition;
	};
}

std::stack<AstNodes::CodeBlock *> blocksStack;

#endif //AST_HH
