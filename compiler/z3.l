%{
#include <stdlib.h>
#include <string.h>


#include "def.tab.hh"

extern int yylineno;
int yyerror(const char *msg, ...);

%}
%%
\/\/			{/*printf("komentarz\n");*/ return COMMENT_ONELINE;}
\/\*			{/*printf("komentarz\n");*/ return COMMENT_BEGIN;}
\*\/			{/*printf("koniec komentarza\n");*/ return COMMENT_END;}
\=\=			{/*printf("operator==\n");*/return OP_COMPARE;}
\+\+			{/*printf("operator++\n");*/return OP_INCREMENT;}
\-\-			{/*printf("operator--\n");*/return OP_DECREMENT;}

\<\=			{return OP_LE;}
\>\=			{return OP_GE;}

\+			{/*printf("operator+\n");*/return '+';} 
\=			{/*printf("operator=\n");*/return '=';} 

\&\&			{return OP_AND;}
\|\|			{return OP_OR;}
\!			{return '!';}

\<			{return '<';}
\>			{return '>';}

\,			{return ',';}
\*			{return '*';}
\(			{return '(';}
\)			{return ')';}
\[			{return '[';}
\]			{return ']';}
\{			{return '{';}
\}			{return '}';}
\/			{return '/';}
\%			{return '%';}
\-			{return '-';}
\^			{return '^';}
\|			{return '|';}
\&			{return '&';}

\?			{return '?';}
\:			{return ':';}

if					{/*printf("if\n");*/return IF;}
else					{/*printf("else\n");*/return ELSE;}
while				{/*printf("while\n");*/return WHILE;}
for					{/*printf("for\n");*/return FOR;}
do					{/*printf("do\n");*/return DO;}

int					{/*printf("type:int\n");*/ return TYPE_INT;}
float				{/*printf("type:float\n");*/ return TYPE_FLOAT;}

[1-9][0-9]*\.[0-9]*		{yylval.fval = atof(yytext); return LR;}
[0-9]+				{yylval.ival = atoi(yytext); return LC;}
	
[A-Za-z_][A-Za-z0-9_]*	{yylval.text = strdup(yytext); return ID;}
\ |\t				{;}
\;					{return ';';}
\n					{yylineno++;}
.					{yyerror("Błąd leksykalny\n");}
%%

int yyerror(const char *msg, ...)
{
	printf("%d: %s\n", yylineno, msg);
	exit(1);
}

