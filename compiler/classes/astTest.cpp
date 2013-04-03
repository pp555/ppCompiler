#include <iostream>

#include "ast.hh"

int main()
{
	AstNodes::Declaration def(NumInt, "a");
	def.codeGen();
	
	AstNodes::Number n(6);
	AstNodes::Variable v("a");
	AstNodes::Assignment a(&v, &n);
	std::cout << a.codeGen();
	
	
	
	
	
	AstNodes::ArithmeticOperation add("+", &n, &v);
	std::cout << add.codeGen();
	
	
}
