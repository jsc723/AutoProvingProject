#pragma once
#include "utils.h"
class Lexer
{
public:
	Lexer();
	~Lexer();
	static vector< deque<string> >  tokenlize(char src[]);
};

