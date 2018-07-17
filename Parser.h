#pragma once
#include "utils.h"
class Node {
public:
	virtual string toString() = 0;
};

class AtomNode : Node {
public:
	AtomNode(string value) {
		this->value = value;
	}
	string toString() {
		return value;
	}
	string value;
};

class UniOpNode : Node {
public:
	UniOpNode(AtomNode *child) : child(child) {};
	string toString() {
		return value + child->toString();
	}
	AtomNode *child;
	string value;
	
};

class NotNode : UniOpNode {
public:
	NotNode(AtomNode *child) : UniOpNode(child) {
		value = "!";
	}
};

class BiOpNode : Node {
public:
	BiOpNode(AtomNode *left, AtomNode *right) {};
	string toString() {
		return "(" + left->toString() + ")" + value + "(" + right->toString() + ")";
	}
	string value;
	AtomNode *left, *right;
};

class AndOpNode : BiOpNode {
public:
	AndOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		value = "^";
	}
};

class OrOpNode : BiOpNode {
public:
	OrOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		value = "v";
	}
};

class IfOpNode : BiOpNode {
public:
	IfOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		value = "->";
	}
};

class EqOpNode : BiOpNode {
public:
	EqOpNode(AtomNode *left, AtomNode *right) :
		BiOpNode(left, right) {
		value = "<->";
	}
};

class Parser
{
public:
	Parser();
	~Parser();
};

