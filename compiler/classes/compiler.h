#ifndef COMPILER_H
#define COMPILER_H

#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>

using namespace std;

typedef std::pair<std::string, int> pairStringInt;

class Address
{
public:
	Address() : i(-1)
	{}
	int next()
	{
		return ++i;
	}
	int current() const
	{
		return i;
	}
private:
	int i;
};

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
	std::string current() const
	{
		std::stringstream stream;
		stream << 't' << i;
		return stream.str();
	}

private:
	int i;
};

class SymbolsMap : public std::map<std::string, int> 
{
public:
	SymbolsMap()
	{
		
	}
	
	int operator[](const std::string &key)
	{
		if(this->find(key) == this->end())
		{
			cerr << "undefined symbol:\t" << key << endl;
			exit(1);
		}
		
		return std::map<std::string, int>::operator [](key);
	}
private:


};

class Compiler
{
public:
	Compiler()
	{
		outFile.open("outFile.txt", ios::out);
		asmFile.open("app.asm", ios::out);
	}

	~Compiler()
	{
		outFile.close();
		asmFile.close();
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
		symbols.insert(pairStringInt(tempIdentifier.current(), addr.next()));

		if(elem2->type() == Element::Identifier)
			asmFile << "MOV R1,#" << symbols[elem2->toString()] << endl;
		else
			asmFile << "MOV R1," << elem2->toString() << endl;

		if(elem1->type() == Element::Identifier)
			asmFile << "MOV R2,#" << symbols[elem1->toString()] << endl;
		else
			asmFile << "MOV R2," << elem1->toString() << endl;

		asmFile << operatorAsmName(op) << " R1,R2" << endl;
		asmFile << "MOV #" << symbols[tempIdentifier.current()] << ",R1" << endl;

		delete elem1;
		delete elem2;
	}

	void assignment()
	{
		std::cout << "assignment" << endl;

		Element *elem1 = elements.top();
		elements.pop();
		Element *elem2 = elements.top();
		elements.pop();

		asmFile << "MOV ";

		if(elem2->type() == Element::Identifier)
			asmFile << "#" << symbols[elem2->toString()];
		else
			asmFile << elem2->toString();

		asmFile << ",";

		if(elem1->type() == Element::Identifier)
			asmFile << "#" << symbols[elem1->toString()] << endl;
		else
			asmFile << elem1->toString() << endl;

		delete elem1;
		delete elem2;

	}

	void declaration(Number::NumberType type)
	{
		cout << "declaration of: " << type << endl;
		symbols.insert(pairStringInt(elements.top()->toString(), addr.next()));
		
		delete elements.top();
		elements.pop();
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
	
	void printSymbols()
	{
		std::cout << "ident\tadres" << endl;
		for(std::map<std::string, int>::iterator it = symbols.begin(); it != symbols.end(); ++it)
		{
			std::cout << it->first << "\t" << it->second << endl;
		}
	}

private:
	std::string operatorAsmName(const std::string &op)
	{
		if("+" == op)
			return "ADD";
		if("-" == op)
			return "SUB";
		if("*" == op)
			return "MUL";
		if("/" == op)
			return "DIV";
		
		std::cerr << "converting to asm:\tunnkonwn operator" << endl;
		exit(1);
	}

	std::stack<Element*> elements;
	SymbolsMap symbols;
	Address addr;
	ofstream outFile;
	ofstream asmFile;
	TempIdentifier tempIdentifier;
};

#endif //COMPILER_H
