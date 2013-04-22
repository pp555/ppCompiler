#ifndef ELEMENTSSTACK_HH
#define ELEMENTSSTACK_HH

#include <stack>

#include "AstNodes/AstNode.hh"

class ElementsStack
{
public:
	ElementsStack()
	{}
	
	AstNodes::AstNode *get()
	{
		AstNodes::AstNode *node = _elements.top();
		_elements.pop();
		return node;
	}
	
	void add(AstNodes::AstNode *node)
	{
		_elements.push(node);
	}
	
	bool isEmpty()
	{
		return _elements.empty();
	}
private:
	std::stack<AstNodes::AstNode*> _elements;
};


#endif
