#ifndef NUMBER_HH
#define NUMBER_HH

#include <sstream>

#include "../enums.hh"

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
}

#endif
