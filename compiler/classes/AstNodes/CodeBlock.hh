#ifndef CODEBLOCK_HH
#define CODEBLOCK_HH

#include <string>
#include <sstream>
#include <list>

#include "../enums.hh"
#include "AstNode.hh"

namespace AstNodes
{
	class CodeBlock : public AstNode
	{
	public:
		CodeBlock() : AstNode(NodeType::CodeBlock)
		{
		}
		
		std::string codeGen()
		{
			std::cout << "gen code for block\n";
			
			std::stringstream result;
			
			for(std::list<AstNode*>::iterator it = _blockContent.begin(); it != _blockContent.end(); ++it)
			{
				result << (*it)->codeGen();
			}
			
			return result.str();
		}
		
		virtual std::string toString(){return "CodeBlock";};
		
		void addStmt(AstNode *node)
		{
			_blockContent.push_back(node);
		}
		
		int stmtCount() const
		{
			return _blockContent.size();
		}
		
	private:
		std::list<AstNode*> _blockContent;
		
	};
}

#endif
