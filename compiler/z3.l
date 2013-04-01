%{
#include <stdlib.h>
#include <string.h>

#include "def.tab.h"

extern int yylineno;

int yyerror(char *msg,...);

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

if			{/*printf("if\n");*/return IF;}
while		{/*printf("while\n");*/return WHILE;}
for			{/*printf("for\n");*/return FOR;}

int			{/*printf("type:int\n");*/ return TYPE_INT;}
float			{/*printf("type:float\n");*/ return TYPE_FLOAT;}

[1-9][0-9]*\.[0-9]*		{yylval.fval = atof(yytext); printf("liczba(float) %f\n", yylval.fval); return LR;}
[1-9][0-9]*		{printf("liczba\n"); yylval.ival = atoi(yytext); return LC;}	
[A-Za-z_][A-Za-z0-9_]*	{printf("nazwa\n"); yylval.text = strdup(yytext); return ID;}
\ |\t			{;}
\;			{return ';';}
\n			{yylineno++;}
.			{yyerror("Błąd leksykalny\n");}
%%
int yyerror(char *msg,...)
{
	printf("%d: %s", yylineno, msg);
	exit(1);
}

