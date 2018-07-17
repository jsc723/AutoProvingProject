#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
using namespace std;
#define readNewLine(in, line, ss)\
getline(in, line); \
ss.clear();  ss.str(line);

int main(int argc, char *argv[]) {
	deque<string> tokens = Lexer::tokenlize("test.txt");
	
	system("pause");
	return 0; 
}