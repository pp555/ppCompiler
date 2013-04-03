#ifndef AST_HH
#define AST_HH

#include <string>
#include <sstream>
#include <map>

#define ENDLINE "\n"

typedef std::string Code;//TODO: vectorof strings

std::map<std::string, int> symbols;//TODO: class Symbol

enum NumberType
{
	NumInt,
	NumFloat
};

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
	
	class Number : public AstNode
	{
	public:
		Number(double value) : _numberType(NumFloat)
		{
			_val.fval = value;
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
		Variable(const std::string &name, int address, NumberType type)//TODO: initialize
		{
			
		}
		
		Code codeGen()
		{
			std::stringstream result;
			result << "#" << symbols[name];
			return result.str();;
		}
	private:
		std::string name;
	};
	
	
	class Assignment : public AstNode
	{
	public:
		Assignment(AstNode *lValue, AstNode *rValue) : lValue(lValue), rValue(rValue)
		{
		}
		
		Code codeGen()
		{
			return "MOV " + lValue->codeGen() + "," + rValue->codeGen() + ENDLINE;
		}
		
	private:
		AstNode *lValue;
		AstNode *rValue;
	};
}


#endif //AST_HH
