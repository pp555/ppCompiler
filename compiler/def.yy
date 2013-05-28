%{
#include <iostream>
#include <fstream>
#include <string>

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

%token <btype> TYPE_BOOL
%token <bool_true> BOOL_TRUE
%token <bool_false> BOOL_FALSE

%token <ival> LC
%token <fval> LR

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

%left OP_COMPARE OP_LE OP_GE '<' '>'
%left OP_OR
%left OP_AND
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
	|sub_block							{}
	;
if_stmt
	:IF '(' condition ')' sub_block										{AstNodes::AstNode *body = elements.get();AstNodes::AstNode *cond = elements.get();elements.add(new AstNodes::IfStmt(body, cond));}
	|IF '(' condition ')' sub_block ELSE sub_block						{AstNodes::AstNode *elseBody = elements.get();AstNodes::AstNode *body = elements.get();AstNodes::AstNode *cond = elements.get();elements.add(new AstNodes::IfStmt(body, cond, elseBody));}
	/*
	|IF '(' complex_condition ')' sub_block								{AstNodes::AstNode *body = elements.get();AstNodes::AstNode *cond = elements.get();elements.add(new AstNodes::IfStmt(body, cond));}
	|IF '(' complex_condition ')' sub_block ELSE sub_block				{AstNodes::AstNode *elseBody = elements.get();AstNodes::AstNode *body = elements.get();AstNodes::AstNode *cond = elements.get();elements.add(new AstNodes::IfStmt(body, cond, elseBody));}
	*/
	;
	/*
complex_condition
	:condition OP_AND condition			{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::ComplexCondition(AstNodes::ComplexCondition::And, lValue, rValue));}
	|condition OP_AND complex_condition	{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::ComplexCondition(AstNodes::ComplexCondition::And, lValue, rValue));}
	|condition OP_OR condition			{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::ComplexCondition(AstNodes::ComplexCondition::Or, lValue, rValue));}
	|condition OP_OR complex_condition	{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::ComplexCondition(AstNodes::ComplexCondition::Or, lValue, rValue));}
	|'(' complex_condition ')'			{std::cout << "cc1\n";}
	;
	*/
condition
	:wyr								{}
	|log_wyr							{}
	|wyr '<' wyr						{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison("<", lValue, rValue));}
	|wyr '>' wyr						{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison(">", lValue, rValue));}
	|wyr OP_LE wyr						{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison("<=", lValue, rValue));}
	|wyr OP_GE wyr						{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison(">=", lValue, rValue));}
	|wyr OP_COMPARE wyr					{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison("==", lValue, rValue));}
	|wyr '!' '=' wyr					{AstNodes::AstNode *rValue = elements.get();AstNodes::AstNode *lValue = elements.get();elements.add(new AstNodes::Comparison("!=", lValue, rValue));}
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
	|TYPE_BOOL ident						{elements.add(new AstNodes::Declaration(TypeBool, static_cast<AstNodes::Variable*>(elements.get())));}
	;
assign
	:ident '=' wyr							{AstNodes::AstNode *rValue = elements.get();AstNodes::Variable *lValue = static_cast<AstNodes::Variable*>(elements.get());elements.add(new AstNodes::Assignment(lValue, rValue));}
	|ident '=' log_wyr						{AstNodes::AstNode *rValue = elements.get();AstNodes::Variable *lValue = static_cast<AstNodes::Variable*>(elements.get());elements.add(new AstNodes::Assignment(lValue, rValue));}
	;
ident
	:ID										{elements.add(new AstNodes::Variable($1));}
	;


log_wyr
	:log_wyr OP_OR log_skladnik				{elements.add(AstNodes::LogicOperation::createFromStack(AstNodes::LogicOperation::Or));}
	|log_skladnik
	;
	
log_skladnik
	:log_skladnik OP_AND log_czynnik		{elements.add(AstNodes::LogicOperation::createFromStack(AstNodes::LogicOperation::And));}
	|log_czynnik							{}
	;
	
log_czynnik
	/*ident*/
	:BOOL_TRUE								{elements.add(new AstNodes::BoolConstant(true));}
	|BOOL_FALSE								{elements.add(new AstNodes::BoolConstant(false));}
	|'(' log_wyr ')'						{}
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
	
	
	std::ofstream asmFile("app.asm.withlabels", ios::out);
	
	
	while(!elements.isEmpty())
	{
		AstNodes::AstNode *e = elements.get();
		cout << "stack element:\t" << e->toString() << endl;
		asmFile << e->codeGen();
	}
	symbols.printSymbols();

	asmFile.close();
	
	std::cout << "generated asm file with labels" << std::endl;
	
	
	
	std::ifstream inAsmFile("app.asm.withlabels", ios::in);
	std::ofstream outAsmFile("app.asm", ios::out);
	
	std::map<std::string, int> labels;//label name, line number
	std::string line;
	int lineNo = 0;
	while(!inAsmFile.eof())
	{
		getline(inAsmFile,line);
		int colonPos = line.find(':');
		int labelStartPos = 0;
		while(colonPos != string::npos)//add to labels list
		{
			labels.insert(std::pair<std::string, int>(line.substr(labelStartPos, colonPos - labelStartPos), lineNo));
			labelStartPos = colonPos + 1;
			colonPos = line.find(':', labelStartPos);
		}
		lineNo++;
	}
std::cout << "labels\n";
		for(std::map<std::string, int>::iterator it = labels.begin(); it != labels.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second << std::endl;
		}
	
	inAsmFile.clear();
	inAsmFile.seekg(0, inAsmFile.beg);
	while(true)
	{
		getline(inAsmFile,line);
		
		int colonPos = line.find(':');
		if(colonPos != string::npos)//add to labels list
		{
			line = line.substr(colonPos + 1);
		}
		
		for(std::map<std::string, int>::iterator it = labels.begin(); it != labels.end(); ++it)
		{
			int labelPos = line.find(it->first);
			if(labelPos != string::npos)
			{
				std::stringstream intStream;
				intStream << it->second;
				line.replace(labelPos, it->first.size(), intStream.str());
			}
		}
		
		if(!inAsmFile.eof())
			outAsmFile << line << std::endl;
		else
			break;
	}
	
	
	inAsmFile.close();
	outAsmFile.close();

	return 0;
}

