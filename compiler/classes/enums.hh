#ifndef ENUMS_HH
#define ENUMS_HH

enum NumberType
{
	None,
	NumInt,
	NumFloat,
	TypeBool
};

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
		Comparison,			//7
		ComplexCondition,	//8
		BoolConstant,		//9
		LogicOperation,		//10
		ArrayDeclaration,	//11
		Indexer,			//12
		ArrayVariable,		//13
		PreparedCode,		//14
		WhileStmt,			//15
		ForStmt				//16
	};
};

#endif
