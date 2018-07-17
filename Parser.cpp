#include "Parser.h"



Parser::Parser()
{
}


Parser::~Parser()
{
}

TreeNode *Parser::parse(deque<string> &tokens) {
	if (tokens.size() == 0)
		return NULL;
	if (tokens.size() == 1 && regex_match(tokens[0], atomReg)) {
		return new AtomNode(tokens[0]);
	}
	return NULL;
}