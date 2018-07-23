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
struct ProofLine {
	set<int> dependence;
	int lineNum;
	TreeNode *formula;
	string tag;
};
class TreeNode {
public:
	virtual string toString() = 0;
	virtual bool eval() = 0;
	virtual void assign(string symbol, bool value) = 0;
	virtual void all_symbols(set<string> &symbols) = 0;
	virtual bool equals(TreeNode *tree) = 0;
	virtual bool contains(TreeNode *tree) = 0;
	virtual bool pgen(vector<ProofLine> &proof, int start) = 0;
	string type;
};

class AtomNode : public TreeNode {
public:
	AtomNode(string symbol) :TreeNode() {
		this->symbol = symbol;
		this->value = false;
		this->type = "AtomNode";
	}
	string toString() {
		return symbol;
	}
	void assign(string symbol, bool value) {
		if (this->symbol == symbol) {
			this->value = value;
		}
	}
	void all_symbols(set<string> &symbols) {
		symbols.insert(symbol);
	}
	bool equals(TreeNode *tree) {
		if (tree->type == this->type) {
			AtomNode *t = dynamic_cast<AtomNode *>(tree);
			return t->symbol == this->symbol;
		}
		return false;
	}
	bool contains(TreeNode *tree) {
		return equals(tree);
	}
	bool eval() { return value; }
	bool pgen(vector<ProofLine> &proof, int start) {
		for (int i = start; i < proof.size(); i++) {

		}
	}
	string symbol;
	bool value;
};

class UniOpNode : public TreeNode {
public:
	UniOpNode(TreeNode *child) : child(child) {};
	string toString() {
		return symbol + child->toString();
	}
	virtual void assign(string symbol, bool value) {
		child->assign(symbol, value);
	}
	void all_symbols(set<string> &symbols) {
		child->all_symbols(symbols);
	}
	bool equals(TreeNode *tree) {
		if (tree->type == this->type) {
			TreeNode *c = dynamic_cast<UniOpNode *>(tree)->child;
			return child->equals(c);
		}
		return false;
	}
	bool contains(TreeNode *tree) {
		if (equals(tree))
			return true;
		return child->contains(tree);
	}
	TreeNode *child;
	string symbol;
};

class NotNode : public UniOpNode {
public:
	NotNode(TreeNode *child) : UniOpNode(child) {
		symbol = "!";
		this->type = "NotNode";
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
		return "(" + left->toString() + symbol + right->toString() + ")";
	}
	virtual void assign(string symbol, bool value) {
		left->assign(symbol, value);
		right->assign(symbol, value);
	}
	void all_symbols(set<string> &symbols) {
		left->all_symbols(symbols);
		right->all_symbols(symbols);
	}
	bool equals(TreeNode *tree) {
		if (tree->type == this->type) {
			TreeNode *cl = dynamic_cast<BiOpNode *>(tree)->left;
			TreeNode *cr = dynamic_cast<BiOpNode *>(tree)->right;
			return left->equals(cl) && right->equals(cr);
		}
		return false;
	}
	bool contains(TreeNode *tree) {
		if (equals(tree))
			return true;
		return left->contains(tree) || right->contains(tree);
	}
	string symbol;
	TreeNode *left, *right;
};

class AndOpNode : public BiOpNode {
public:
	AndOpNode(TreeNode *left, TreeNode *right) :
		BiOpNode(left, right) {
		symbol = "^";
		this->type = "AndOpNode";
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
		this->type = "OrOpNode";
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
		this->type = "IfOpNode";
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
		this->type = "EqOpNode";
	}
	bool eval() {
		return left->eval() == right->eval();
	}
};
