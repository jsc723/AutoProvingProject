#include "Lexer.h"



Lexer::Lexer()
{
}


Lexer::~Lexer()
{
}

deque<string> Lexer::tokenlize(char src[])
{
	ifstream in(src);
	stringstream ss;
	string line;
	
	deque<string> tokens;
	
	while (!in.eof()) {
		getline(in, line);
		auto words_begin =
			sregex_iterator(line.begin(), line.end(), allReg);
		auto words_end = sregex_iterator();
		for (sregex_iterator i = words_begin; i != words_end; ++i) {
			smatch match = *i;
			string match_str = match.str();
			cout << match_str << '\n';
			tokens.push_back(match_str);
		}
	}
	return tokens;
}
