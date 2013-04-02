#ifndef COMPILER_H
#define COMPILER_H

#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>

using namespace std;

extern int yylineno;

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

class Variable //TODO: use in SymbolsMap
{
public:
	Variable() : address(-1), type(Number::NumInt)//TODO: remove default type
	{}
	Variable(int address, Number::NumberType type = Number::NumInt) : address(address), type(type)//TODO: remove default type
	{}
	Variable(const Variable &v) : address(v.getAddress()), type(v.getType())
	{}
	
	int getAddress() const
	{
		return address;
	}
	
	Number::NumberType getType() const
	{
		return type;
	}
	
private:
	int address;
	Number::NumberType type;
	int arraySize; //TODO: arrays
};
ostream & operator <<(ostream &str, const Variable &v)
{
	str << v.getAddress() << "\t" << v.getType();
	return str;
}

typedef std::pair<std::string, Variable> pairStringVariable;

class SymbolsMap : public std::map<std::string, Variable> 
{
public:
	SymbolsMap()
	{
		
	}
	
	Variable operator[](const std::string &key)
	{
		if(this->find(key) == this->end())
		{
			cerr << yylineno << ":\tundefined symbol:\t" << key << endl;
			exit(1);
		}
		
		return std::map<std::string, Variable>::operator [](key);
	}
	
	void insert(const std::string &key, int address, Number::NumberType type = Number::NumInt)
	{
		std::pair<iterator,bool> result = std::map<std::string, Variable>::insert(pairStringVariable(key, Variable(address, type)));
		
		if(!result.second)//already existed
		{
			cerr << yylineno << ":\tsymbol redefinition:\t" << key << endl;
			exit(1);
		}
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
	
	void ifStmt()
	{
		//TODO
		cout << "if:\t";
		Element *elem = elements.top();
		elements.pop();
		
		
		cout << elem->toString();
		cout << endl;
		delete elem;
	}

	void operatorFound(const std::string &op)
	{
		cout << "operator found:\t" << op << endl;
		if(elements.empty())
		{
			cerr << yylineno << ":\terror: stack is empty!" << endl;
			return;
		}

		Element *elemRight = elements.top();
		elements.pop();
		Element *elemLeft = elements.top();
		elements.pop();
		
		outFile << tempIdentifier.next() << '=' << elemLeft->toString() << op << elemRight->toString() << endl;
		push(new Identifier(tempIdentifier.current()));
		symbols.insert(tempIdentifier.current(), addr.next());//TODO: add type

		asmFile << "MOV R1," << expandIfSymbol(elemLeft) << endl
				<< "MOV R2," << expandIfSymbol(elemRight) << endl
				<< operatorAsmName(op) << " R1,R2" << endl
				<< "MOV #" << symbols[tempIdentifier.current()].getAddress() << ",R1" << endl;

		delete elemRight;
		delete elemLeft;
	}

	void assignment()
	{
		std::cout << "assignment" << endl;

		Element *elemRight = elements.top();
		elements.pop();
		Element *elemLeft = elements.top();
		elements.pop();

		asmFile << "MOV "
				<< expandIfSymbol(elemLeft) << ","
				<< expandIfSymbol(elemRight) << endl;

		delete elemRight;
		delete elemLeft;

	}

	void declaration(Number::NumberType type)
	{
		cout << "declaration of: " << type << endl;
		symbols.insert(elements.top()->toString(), addr.next(), type);
		
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
					case Number::NumInt:
						cout << "int:\t" << e->ival() << endl;
						break;
					case Number::NumFloat:
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
		for(std::map<std::string, Variable>::iterator it = symbols.begin(); it != symbols.end(); ++it)
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
		
		std::cerr << yylineno << ":\tconverting to asm:\tunnkonwn operator:\t" << op << endl;
		exit(1);
	}

	std::string expandIfSymbol(Element *element)
	{
		if(element->type() == Element::Identifier)
		{
			stringstream result;
			result << "#" << symbols[element->toString()].getAddress();
			return result.str();
		}
		else
			return element->toString();
	}
	
	std::stack<Element*> elements;
	SymbolsMap symbols;
	Address addr;
	ofstream outFile;
	ofstream asmFile;
	TempIdentifier tempIdentifier;
};

#endif //COMPILER_H
