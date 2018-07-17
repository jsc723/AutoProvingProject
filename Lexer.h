#pragma once
#include "utils.h"
class Lexer
{
public:
	Lexer();
	~Lexer();
	static deque<string> tokenlize(char src[]);
};

