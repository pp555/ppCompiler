%{
#include <string.h>
#include <stdio.h>
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
%token <while> WHILE
%token <for> FOR

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

%start instruction

%%
instruction
	:if					{}
	|for				{}
	|while				{}
	|declaration ';'	{}
	|wyr ';'			{printf("wyr\n");}
	|';'				{}
	;
if
	:IF '(' wyr ')'	instruction	{printf("if\n");}
	;
for
	:FOR '(' declaration ';' wyr ';' wyr ')' instruction		{printf("for\n");}
	;
while
	:WHILE '(' wyr ')' instruction	{printf("while\n");}
	;
declaration
	:TYPE_INT ID '=' wyr		{printf("int declaration and definition\n");}
	|TYPE_FLOAT ID '=' wyr		{printf("float declaration and definition\n");}
	|TYPE_INT ID		{printf("int declaration\n");}
	|TYPE_FLOAT ID		{printf("float declaration\n");}
	;
wyr
	:wyr OP_COMPARE wyr	{printf("porownanie\n");}
	|wyr OP_LE wyr	{printf("porownanie\n");}
	|wyr OP_GE wyr	{printf("porownanie\n");}
	|wyr '<' wyr	{printf("porownanie\n");}
	|wyr '>' wyr	{printf("porownanie\n");}


	|wyr '+' skladnik	{printf("wyrazenie z + \n");}
	|wyr '-' skladnik	{printf("wyrazenie z - \n");}
	|skladnik		{printf("wyrazenie pojedyncze \n");}
	;
skladnik
	:skladnik '*' czynnik	{printf("skladnik z * \n");}
	|skladnik '/' czynnik	{printf("skladnik z / \n");}
	|czynnik		{printf("skladnik pojedynczy \n");}
	;
czynnik

	:ID OP_INCREMENT	{printf("inkrementacja \n");}
	|ID OP_DECREMENT	{printf("dekrementacja \n");}

	|ID			{printf("czynnik znakowy\n");} 
	|LC			{printf("czynnik liczbowy\n");}
	|'(' wyr ')'		{printf("wyrazenie w nawiasach\n");}
	;
%%
int main(int argc, char *argv[])
{
	yyparse();
	return 0;
}

