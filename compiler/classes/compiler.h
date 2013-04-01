#ifndef COMPILER_H
#define COMPILER_H

#include <stack>
#include <fstream>
#include <sstream>

using namespace std;

class TempIdentifier
{
public:
	TempIdentifier() : i(-1)
	{}
	std::string next()
	{
		std::stringstream stream;
		stream << 't' << ++i;
		return stream.str();
	}
	std::string current()
	{
		std::stringstream stream;
		stream << 't' << i;
		return stream.str();
	}

private:
	int i;
};

class Compiler
{
public:
	Compiler()
	{
		outFile.open("outFile.txt", ios::out);
		
	}

	~Compiler()
	{
		outFile.close();
	}

	void operatorFound(const std::string &op)
	{
		cout << "operator found:\t" << op << endl;
		if(elements.empty())
		{
			cerr << "error: stack is empty!" << endl;
			return;
		}

		Element *elem1 = elements.top();
		elements.pop();
		Element *elem2 = elements.top();
		elements.pop();
		
		outFile << tempIdentifier.next() << '=' << elem2->toString() << op << elem1->toString() << endl;
		push(new Identifier(tempIdentifier.current()));

		delete elem1;
		delete elem2;
	}

	void push(Element* e)
	{
		elements.push(e);
	}

	void checkStack()
	{
		while(!elements.empty())
		{
			cerr << "stos nie jest pusty!" << endl;
			Element *elem = elements.top();
		
			switch(elem->type())
			{
			case Element::Identifier:
				{
					Identifier *e = static_cast<Identifier*>(elem);
					cout << "identifier:\t" << e->name() << endl;
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
	}

private:
	std::stack<Element*> elements;
	ofstream outFile;
	TempIdentifier tempIdentifier;
};

#endif //COMPILER_H
