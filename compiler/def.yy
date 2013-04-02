%{
#include <iostream>
#include <fstream>
#include <string>

#include "classes/element.h"
#include "classes/identifier.h"
#include "classes/number.h"
#include "classes/compiler.h"

FILE *npnfile;

Compiler compiler;

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
	:blok							{printf("blok\n");fprintf(npnfile, "\n");}
	|bloki blok						{printf("bloki\n");fprintf(npnfile, "\n");}
	|sub_block
	;
blok
	:declaration ';'					{}
	|assign ';'						{fprintf(npnfile, ";");}
	|if_stmt							{}
	|do_while_stmt						{}
	|while_stmt						{}
	|for_stmt							{}
	|function_call						{}
	;
if_stmt
	:IF '(' wyr ')' control_body										{compiler.ifStmt();}
	|IF '(' wyr ')' sub_block ELSE control_body							{printf("if-else\n");}
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
	:'{' bloki '}'							{}
	;

declaration
	:TYPE_INT ID '=' wyr				{printf("int declaration and definition\n");}
	|TYPE_FLOAT ID '=' wyr				{printf("float declaration and definition\n");}
	|TYPE_INT ident						{printf("int declaration\n"); compiler.declaration(Number::NumInt);}
	|TYPE_FLOAT ident						{printf("float declaration\n"); compiler.declaration(Number::NumFloat);}
	;
assign
	:ident '=' wyr							{printf("przypisanie\n");fprintf(npnfile, "=");compiler.assignment();}
	;
ident
	:ID										{fprintf(npnfile, $1);fprintf(npnfile, " ");compiler.push(new Identifier($1));}
	;
wyr
	:wyr '+' skladnik						{printf("wyrazenie z + \n");fprintf(npnfile, "+");compiler.operatorFound("+");}
	|wyr '-' skladnik						{printf("wyrazenie z - \n");fprintf(npnfile, "-");compiler.operatorFound("-");}
	|skladnik								{printf("wyrazenie pojedyncze \n");}
	;
skladnik
	:skladnik '*' czynnik					{printf("skladnik z * \n");fprintf(npnfile, "*");compiler.operatorFound("*");}
	|skladnik '/' czynnik					{printf("skladnik z / \n");fprintf(npnfile, "/");compiler.operatorFound("/");}
	|czynnik								{printf("skladnik pojedynczy \n");}
	;
czynnik
	:ident								{printf("identyfikator\n");} 
	|LC									{printf("czynnik liczbowy\n");fprintf(npnfile, "%d ", $1);compiler.push(new Number($1));}
	|LR									{printf("czynnik liczbowy (f)\n");fprintf(npnfile, "%f ", $1);compiler.push(new Number($1));}
	|'(' wyr ')'							{printf("wyrazenie w nawiasach\n");}
	;
	
%%

int main(int argc, char *argv[])
{

	npnfile = fopen("out.txt", "w");
	yyparse();
	fclose(npnfile);

	compiler.checkStack();
	compiler.printSymbols();

	return 0;
}

