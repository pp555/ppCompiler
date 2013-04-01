%{
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "inc/element.h"
#include "inc/identifier.h"
#include "inc/number.h"

FILE *npnfile;

extern "C" int yylex();
extern "C" int yyerror(const char *msg, ...);
using namespace std;

std::stack<Element*> elements;

void operatorFound(const std::string &op)
{
	cout << "operator found" << endl;
	if(elements.empty())
	{
		cerr << "error: stack is empty!" << endl;
		return;
	}

	ofstream outFile("outFile.txt", ios::out);

	Element *elem = elements.top();
		
	switch(elem->type())
	{
	case Element::Identifier:
		{
			Identifier *e = static_cast<Identifier*>(elem);
			cout << "id:\t" << e->name() << endl;
		}
		break;
	case Element::Number:
		{
			Number *e = static_cast<Number*>(elem);
			switch(e->numberType())
			{
			case Number::NUM_INT:
				cout << "int:\t" << e->ival() << endl;
				outFile << e->ival();
				break;
			case Number::NUM_FLOAT:
				cout << "float:\t" << e->fval() << endl;
				break; 
			}
		}
		break;
	}

	elements.pop();
	delete elem;
}

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
	:IF '(' wyr ')' control_body										{printf("if\n");}
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
	:TYPE_INT ID '=' wyr					{printf("int declaration and definition\n");}
	|TYPE_FLOAT ID '=' wyr					{printf("float declaration and definition\n");}
	|TYPE_INT ID							{printf("int declaration\n");}
	|TYPE_FLOAT ID							{printf("float declaration\n");}
	;
assign
	:ident '=' wyr							{printf("przypisanie\n");fprintf(npnfile, "=");}
	;
ident
	:ID									{printf("$1\n");fprintf(npnfile, $1);fprintf(npnfile, " ");elements.push(new Identifier($1));}
	;
wyr
	:wyr '+' skladnik						{printf("wyrazenie z + \n");fprintf(npnfile, "+");operatorFound("+");}
	|wyr '-' skladnik						{printf("wyrazenie z - \n");fprintf(npnfile, "-");operatorFound("-");}
	|skladnik								{printf("wyrazenie pojedyncze \n");}
	;
skladnik
	:skladnik '*' czynnik					{printf("skladnik z * \n");fprintf(npnfile, "*");operatorFound("*");}
	|skladnik '/' czynnik					{printf("skladnik z / \n");fprintf(npnfile, "/");operatorFound("/");}
	|czynnik								{printf("skladnik pojedynczy \n");}
	;
czynnik
	:ident								{printf("identyfikator\n");} 
	|LC									{printf("czynnik liczbowy\n");fprintf(npnfile, "%d ", $1);elements.push(new Number($1));}
	|LR									{printf("czynnik liczbowy (f)\n");fprintf(npnfile, "%d ", $1);elements.push(new Number($1));}
	|'(' wyr ')'							{printf("wyrazenie w nawiasach\n");}
	;
	
%%

int main(int argc, char *argv[])
{

	npnfile = fopen("out.txt", "w");
	yyparse();
	fclose(npnfile);

	while(!elements.empty())
	{
		cout << "stos nie jest pusty!" << endl;
		Element *elem = elements.top();
		
		switch(elem->type())
		{
		case Element::Identifier:
			{
				Identifier *e = static_cast<Identifier*>(elem);
				cout << e->name() << endl;
			}
			break;
		case Element::Number:
			{
				Number *e = static_cast<Number*>(elem);
				switch(e->numberType())
				{
				case Number::NUM_INT:
					cout << "int:\t" << e->ival() << endl;
					break;
				case Number::NUM_FLOAT:
					cout << "float:\t" << e->fval() << endl;
					break; 
				}
			}
			break;
		}

		elements.pop();
		delete elem;
	}

	return 0;
}

