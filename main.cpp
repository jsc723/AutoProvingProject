#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "Solver.h"
using namespace std;
#define readNewLine(in, line, ss)\
getline(in, line); \
ss.clear();  ss.str(line);

int main(int argc, char *argv[]) {
	deque<string> tokens = Lexer::tokenlize("test.txt");
	TreeNode *tree = Parser::parse(tokens);
	cout << "reconstruct: \n" << tree->toString() << endl;
	set<string> symbols;
	tree->all_symbols(symbols);
	for (auto i : symbols) {
		cout << i << " " << endl;
	}
	cout << Solver::provable(tree, true) << endl;
	system("pause");
	return 0; 
}