#ifndef AST_HH
#define AST_HH

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>

#include "TempIdentifier.hh"
#include "Address.hh"

#define ENDLINE "\n"

typedef std::string Code;//TODO: vectorof strings

/*extern*/ int yylineno;//TODO

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

	void insert(const std::string &name, int address)//TODO: add address in parameter/make addres in body
	{
		std::pair<std::map<std::string, int>::iterator,bool> result = _symbols.insert(std::pair<std::string, int>(name, address));
		
		if(!result.second)//already existed
		{
			std::cerr << yylineno << ":\tsymbol redefinition:\t" << name << std::endl;
			exit(1);
		}
	}
	
	int operator[](const std::string &name)
	{
		if(_symbols.find(name) == _symbols.end())
		{
			std::cerr << yylineno << ":\tundefined symbol:\t" << name << std::endl;
			exit(1);
		}
		return _symbols[name];
	}
	
	

private:
	std::map<std::string, int> _symbols;
};


SymbolsMap symbols;
TempIdentifier tempIdentifier;
Address addr;

namespace AstNodes
{
		
	class AstNode
	{
	public:
		AstNode()
		{}
		virtual ~AstNode()
		{}
		virtual Code codeGen() = 0;
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
		
		ArithmeticOperation(const std::string &op, AstNode *lValue, AstNode *rValue) : _lValue(lValue), _rValue(rValue), _operator(Unnknown)
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
		ArithmeticOperation(Operator op, AstNode *lValue, AstNode *rValue) : _lValue(lValue), _rValue(rValue), _operator(op)
		{}
		
		Code codeGen()
		{
			symbols.insert(tempIdentifier.next(), addr.next());//TODO: add type
			std::stringstream result;
			result << "MOV R1," << _lValue->codeGen() << std::endl
				<< "MOV R2," << _rValue->codeGen() << std::endl
				<< opAsmName() << " R1,R2" << std::endl
				<< "MOV #" << symbols[tempIdentifier.current()] << ",R1" << std::endl;
			
			return result.str();
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
	
	class Number : public AstNode
	{
	public:
		Number(double value) : _numberType(NumFloat)
		{
			_val.fval = value;
		}
		Number(float value) : _numberType(NumFloat)
		{
			_val.fval = value;
		}
		Number(int value) : _numberType(NumInt)
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
		Variable(const std::string &name) : _name(name)
		{
			
		}
		
		Code codeGen()
		{
			std::stringstream result;
			result << "#" << symbols[_name];
			return result.str();;
		}
	private:
		std::string _name;
	};
	
	
	class Assignment : public AstNode
	{
	public:
		Assignment(AstNode *lValue, AstNode *rValue) : _lValue(lValue), _rValue(rValue)
		{
		}
		
		Code codeGen()
		{
			//TODO: check lvalue (it must be variable)
			return "MOV " + _lValue->codeGen() + "," + _rValue->codeGen() + ENDLINE;
		}
		
	private:
		AstNode *_lValue;
		AstNode *_rValue;
	};
	
	class Declaration : public AstNode
	{
	public:
		Declaration(NumberType type, const std::string &name) : _type(type), _name(name)
		{}
		
		Code codeGen()
		{
			symbols.insert(_name, addr.next());
			return "";//TODO: codeGen returning void, appending code to vector
		}
	private:
		std::string _name;
		NumberType _type;
	};
}


#endif //AST_HH
