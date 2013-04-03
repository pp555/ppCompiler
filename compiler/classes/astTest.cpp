#include <iostream>

#include "ast.hh"

int main()
{
	AstNodes::Number n(5.0);
	std::cout << n.codeGen() << std::endl;
	
	AstNodes::Variable v("a", 0, NumInt);
	std::cout << v.codeGen() << std::endl;
	
	AstNodes::Assignment a(&v, &n);
	std::cout << a.codeGen() << std::endl;
	
	
}
