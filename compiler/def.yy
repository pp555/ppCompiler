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
	:blok							{printf("blok\n");}
	|bloki blok						{printf("bloki\n");}
	|sub_block						{}
	;
blok
	:declaration ';'					{}
	|assign ';'						{}
	|if_stmt							{}
	|do_while_stmt						{}
	|while_stmt						{}
	|for_stmt							{}
	|function_call						{}
	;
if_stmt
	:IF '(' wyr ')' sub_block										{}
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
	:'{' bloki '}'							{elements.add(new AstNodes::CodeBlock());}
	;

declaration
	:TYPE_INT ident						{printf("int declaration\n"); elements.add(new AstNodes::Declaration(NumInt, static_cast<AstNodes::Variable*>(elements.get())));}
	|TYPE_FLOAT ident						{printf("float declaration\n"); elements.add(new AstNodes::Declaration(NumFloat, static_cast<AstNodes::Variable*>(elements.get())));}
	;
assign
	:ident '=' wyr							{printf("przypisanie\n");AstNodes::AstNode *rValue = elements.get();AstNodes::Variable *lValue = static_cast<AstNodes::Variable*>(elements.get());elements.add(new AstNodes::Assignment(lValue, rValue));}
	;
ident
	:ID										{elements.add(new AstNodes::Variable($1));}
	;
wyr
	:wyr '+' skladnik						{printf("wyrazenie z + \n");elements.add(AstNodes::ArithmeticOperation::createFromStack("+"));}
	|wyr '-' skladnik						{printf("wyrazenie z - \n");elements.add(AstNodes::ArithmeticOperation::createFromStack("-"));}
	|skladnik								{printf("wyrazenie pojedyncze \n");}
	;
skladnik
	:skladnik '*' czynnik					{printf("skladnik z * \n");elements.add(AstNodes::ArithmeticOperation::createFromStack("*"));}
	|skladnik '/' czynnik					{printf("skladnik z / \n");elements.add(AstNodes::ArithmeticOperation::createFromStack("/"));}
	|czynnik								{printf("skladnik pojedynczy \n");}
	;
czynnik
	:ident								{printf("identyfikator\n");} 
	|LC									{printf("czynnik liczbowy\n");elements.add(new AstNodes::Number($1));}
	|LR									{printf("czynnik liczbowy (f)\n");elements.add(new AstNodes::Number($1));}
	|'(' wyr ')'							{printf("wyrazenie w nawiasach\n");}
	;
	
%%

int main(int argc, char *argv[])
{
	yyparse();

	//compiler.checkStack();
	
	
	std::ofstream asmFile("app.asm", ios::out);
	
	
	while(!elements.isEmpty())
	{
		cout << "----------   stack element   ----------\n" << endl;
		
		AstNodes::AstNode *e = elements.get();
		asmFile << e->codeGen();
	}
	symbols.printSymbols();

	asmFile.close();

	return 0;
}

