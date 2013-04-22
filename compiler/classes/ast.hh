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

#include "AstNodes/AstNode.hh"
#include "AstNodes/Number.hh"
#include "AstNodes/Variable.hh"
#include "AstNodes/ArithmeticOperation.hh"
#include "AstNodes/Assignment.hh"
#include "AstNodes/Declaration.hh"
#include "AstNodes/CodeBlock.hh"
#include "AstNodes/Comparison.hh"
#include "AstNodes/IfStmt.hh"

extern int yylineno;

SymbolsMap symbols;
TempIdentifier tempIdentifier("%t");
Address addr;
ElementsStack elements;
TempIdentifier label("");
std::stack<AstNodes::CodeBlock *> blocksStack;

#endif //AST_HH
