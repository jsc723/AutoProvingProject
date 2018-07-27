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
	vector< deque<string> > tokens = Lexer::tokenlize("test.txt");
	TreeNode *conclusion = Parser::parse(*(tokens.end() - 1));
	Proof proof;
	set<int> dep;
	cout << "problem: ";
	for (int i = 0; i < tokens.size() - 1; i++) {
		TreeNode *premise = Parser::parse(tokens[i]);
		cout << premise->toString();
		if (i < tokens.size() - 2)
			cout << ", ";
		set<int> d;
		d.insert(i);
		dep.insert(i);
		ProofLine p(d, premise, "P");
		proof.push_back(p);
	}
	cout << " |- " << conclusion->toString() << endl;
	
	
	conclusion->pgen(dep, proof);
	ProofLine::printProof(proof);
	system("pause");
	return 0; 
}