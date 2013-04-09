%{
#include <iostream>
#include <fstream>
#include <string>

//#include "classes/element.h"
//#include "classes/identifier.h"
//#include "classes/number.h"
//#include "classes/compiler.h"

#include "classes/ast.hh"

extern "C" int yylex();
extern "C" int yyerror(const char *msg, ...);
using namespace std;

%}

%union 
{
char *text;
int	ival;
float	fval;
};

%type <text> wyr
%token <text> ID

%token <itype> TYPE_INT
%token <ftype> TYPE_FLOAT

%token <ival> LC
%token <fval> LR

%token <comment_oneline> COMMENT_ONELINE
%token <comment_begin> COMMENT_BEGIN
%token <comment_end> COMMENT_END

%token <if> IF
%token <else> ELSE
%token <while> WHILE
%token <for> FOR
%token <do> DO

%token <increment> OP_INCREMENT
%token <decrement> OP_DECREMENT
%token <compare> OP_COMPARE
%token <le> OP_LE
%token <ge> OP_GE
%token <and> OP_AND
%token <or> OP_OR

//kolejnosc operatorow
%left OP_COMPARE OP_LE OP_GE '<' '>'
%left '+' '-'
%left '*' '/'


%%
bloki
	:blok							{}
	|bloki blok						{}
	|sub_block						{}
	;
blok
	:declaration ';'					{blocksStack.top()->addStmt(elements.get());}
	|assign ';'							{blocksStack.top()->addStmt(elements.get());}
	|if_stmt							{blocksStack.top()->addStmt(elements.get());}
	|do_while_stmt						{}
	|while_stmt							{}
	|for_stmt							{}
	|function_call						{}
	;
if_stmt
	:IF '(' wyr ')' sub_block										{elements.add(new AstNodes::IfStmt(elements.get(), elements.get()));}
	|IF '(' wyr ')' sub_block ELSE sub_block							{printf("if-else\n");}
	;

while_stmt
	:WHILE '(' wyr ')' control_body									{printf("while\n");}
	;
for_stmt
	:FOR '(' declaration ';' wyr ';' wyr ')' control_body					{printf("for\n");}
	;
do_while_stmt
	:DO control_body WHILE ';'				{}
	;

function_call
	:ID '(' arguments ')' ';'				{printf("funkcja z argumentami\n");}
	|ID '(' ')' ';'						{printf("funkcja bez argumentow\n");}
	;
arguments
	:argument								{}
	|argument ',' arguments					{}
	;
argument
	:wyr									{printf("argument\n");}
	;

control_body
	:blok								{}
	|sub_block							{}
	;

sub_block
	: sub_block_begin bloki '}'							{printf("block end\n");elements.add(blocksStack.top());blocksStack.pop();}
	;
sub_block_begin : '{'									{printf("block begin\n");blocksStack.push(new AstNodes::CodeBlock());};

declaration
	:TYPE_INT ident							{elements.add(new AstNodes::Declaration(NumInt, static_cast<AstNodes::Variable*>(elements.get())));}
	|TYPE_FLOAT ident						{elements.add(new AstNodes::Declaration(NumFloat, static_cast<AstNodes::Variable*>(elements.get())));}
	;
assign
	:ident '=' wyr							{AstNodes::AstNode *rValue = elements.get();AstNodes::Variable *lValue = static_cast<AstNodes::Variable*>(elements.get());elements.add(new AstNodes::Assignment(lValue, rValue));}
	;
ident
	:ID										{elements.add(new AstNodes::Variable($1));}
	;
wyr
	:wyr '+' skladnik						{elements.add(AstNodes::ArithmeticOperation::createFromStack("+"));}
	|wyr '-' skladnik						{elements.add(AstNodes::ArithmeticOperation::createFromStack("-"));}
	|skladnik								{}
	;
skladnik
	:skladnik '*' czynnik					{elements.add(AstNodes::ArithmeticOperation::createFromStack("*"));}
	|skladnik '/' czynnik					{elements.add(AstNodes::ArithmeticOperation::createFromStack("/"));}
	|czynnik								{}
	;
czynnik
	:ident								{} 
	|LC									{elements.add(new AstNodes::Number($1));}
	|LR									{elements.add(new AstNodes::Number($1));}
	|'(' wyr ')'							{}
	;
	
%%

int main(int argc, char *argv[])
{
	blocksStack.push(new AstNodes::CodeBlock());
	yyparse();

	//compiler.checkStack();
	
	
	
	
	AstNodes::AstNode *root = elements.get();
	std::string app = root->codeGen();
	
	
	while(!elements.isEmpty())
	{
		std::cerr << "stack is not empty after getting root element\n";
		AstNodes::AstNode *e = elements.get();
		cout << "stack element:\t" << e->toString() << endl;
		std::cout << e->codeGen();
	}
	//symbols.printSymbols();
	
	//TODO:replacing labels
	app.replace(app.find("label.e1:"), 9, "line_no:");
	app.replace(app.find("label.s1:"), 9, "line_no:");

	std::ofstream asmFile("app.asm", ios::out);
	asmFile << app;
	asmFile.close();

	return 0;
}

