#ifndef AST_HH
#define AST_HH

#include <stack>

#include "Symbol.hh"
#include "SymbolsMap.hh"
#include "TempIdentifier.hh"
#include "Address.hh"
#include "enums.hh"
#include "ElementsStack.hh"

#define ENDLINE "\n"

#include "AstNodes/PreparedCode.hh"
#include "AstNodes/AstNode.hh"
#include "AstNodes/Number.hh"
#include "AstNodes/Variable.hh"
#include "AstNodes/ArithmeticOperation.hh"
#include "AstNodes/Assignment.hh"
#include "AstNodes/Declaration.hh"
#include "AstNodes/CodeBlock.hh"
#include "AstNodes/Comparison.hh"
#include "AstNodes/IfStmt.hh"
#include "AstNodes/Loops.hh"
#include "AstNodes/ComplexCondition.hh"
#include "AstNodes/BoolConstant.hh"
#include "AstNodes/LogicOperation.hh"
#include "AstNodes/Array.hh"
#include "AstNodes/Functions.hh"

extern int yylineno;

SymbolsMap symbols;
TempIdentifier tempIdentifier("%t");
Address addr;
ElementsStack elements;
std::stack<AstNodes::CodeBlock *> blocksStack;
std::stack<int> labelsStack;
Address labels;

#endif //AST_HH
