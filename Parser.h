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
	UniOpNode(AtomNode *child) : child(child) {};
	string toString() {
		return symbol + child->toString();
	}
	AtomNode *child;
	string symbol;
};

class NotNode : public UniOpNode {
public:
	NotNode(AtomNode *child) : UniOpNode(child) {
		symbol = "!";
	}
	bool eval() {
		return !child->eval();
	}
};

class BiOpNode : public TreeNode {
public:
	BiOpNode(AtomNode *left, AtomNode *right) {};
	string toString() {
		return "(" + left->toString() + ")" + symbol + "(" + right->toString() + ")";
	}
	string symbol;
	AtomNode *left, *right;
};

class AndOpNode : public BiOpNode {
public:
	AndOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		symbol = "^";
	}
	bool eval() {
		return left->eval() && right->eval();
	}
};

class OrOpNode : public BiOpNode {
public:
	OrOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		symbol = "v";
	}
	bool eval() {
		return left->eval() || right->eval();
	}
};

class IfOpNode : public BiOpNode {
public:
	IfOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		symbol = "->";
	}
	bool eval() {
		return !(left->eval() && !right->eval());
	}
};

class EqOpNode : public BiOpNode {
public:
	EqOpNode(AtomNode *left, AtomNode *right) :
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
};

