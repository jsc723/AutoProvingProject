#pragma once
#include "utils.h"
class TreeNode;
class AtomNode;
class UniOpNode;
class NotNode;
class BiOpNode;
class AndOpNode;
class OrOpNode;
class IfOpNode;
class EqOpNode;
class TreeNode {
public:
	virtual string toString() = 0;
	virtual bool eval() = 0;
};

class AtomNode : public TreeNode {
public:
	AtomNode(string symbol):TreeNode() {
		this->symbol = symbol;
		this->value = false;
	}
	string toString() {
		return symbol;
	}
	bool eval() { return value; }
	string symbol;
	bool value;
};

class UniOpNode : public TreeNode {
public:
	UniOpNode(TreeNode *child) : child(child) {};
	string toString() {
		return symbol + child->toString();
	}
	TreeNode *child;
	string symbol;
};

class NotNode : public UniOpNode {
public:
	NotNode(TreeNode *child) : UniOpNode(child) {
		symbol = "!";
	}
	bool eval() {
		return !child->eval();
	}
};

class BiOpNode : public TreeNode {
public:
	BiOpNode(TreeNode *left, TreeNode *right) :
		left(left), right(right) {};
	string toString() {
		return "(" + left->toString() + ")" + symbol + "(" + right->toString() + ")";
	}
	string symbol;
	TreeNode *left, *right;
};

class AndOpNode : public BiOpNode {
public:
	AndOpNode(TreeNode *left, TreeNode *right) :
		BiOpNode(left, right) {
		symbol = "^";
	}
	bool eval() {
		return left->eval() && right->eval();
	}
};

class OrOpNode : public BiOpNode {
public:
	OrOpNode(TreeNode *left, TreeNode *right) :
		BiOpNode(left, right) {
		symbol = "v";
	}
	bool eval() {
		return left->eval() || right->eval();
	}
};

class IfOpNode : public BiOpNode {
public:
	IfOpNode(TreeNode *left, TreeNode *right) :
		BiOpNode(left, right) {
		symbol = "->";
	}
	bool eval() {
		return !(left->eval() && !right->eval());
	}
};

class EqOpNode : public BiOpNode {
public:
	EqOpNode(TreeNode *left, TreeNode *right) :
		BiOpNode(left, right) {
		symbol = "<->";
	}
	bool eval() {
		return left->eval() == right->eval();
	}
};

class Parser
{
public:
	Parser();
	~Parser();
	static TreeNode *parse(deque<string> &tokens);
private:
	static deque<string>::iterator nextArg(deque<string> &tokens, deque<string>::iterator it);
	static void takeOutPara(deque<string> &tokens);
};

