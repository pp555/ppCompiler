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
%token <fval> LF

%token <comment_oneline> COMMENT_ONELINE
%token <comment_begin> COMMENT_BEGIN
%token <comment_end> COMMENT_END

%token <if> IF
%token <while> WHILE
%token <for> FOR

%token <increment> OP_INCREMENT
%token <decrement> OP_DECREMENT
%token <compare> OP_COMPARE

%left '+' '-'
%left '*' '/'

%%
if
	:IF '(' wyr ')'		{printf("TODO:if\n");}

	;
wyr
	:wyr '+' skladnik	{printf("wyrazenie z + \n");}
	|wyr '-' skladnik	{printf("wyrazenie z - \n");}
	|skladnik		{printf("wyrazenie pojedyncze \n");}
	;
skladnik
	:skladnik '*' czynnik	{printf("skladnik z * \n");}
	|skladnik '/' czynnik	{printf("skladnik z / \n");}
	|czynnik		{printf("skladnik pojedynczy \n");}
	;
czynnik
	:ID			{printf("czynnik znakowy\n");} 
	|LC			{printf("czynnik liczbowy\n");}
	|'(' wyr ')'		{printf("wyrazenie w nawiasach\n");}
	;
%%
int main(int argc, char *argv[])
{
	yyparse();
	return 0;
}

