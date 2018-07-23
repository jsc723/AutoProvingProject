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
	vector<int> args;
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
	virtual bool pgen(set<int> &dep, vector<ProofLine> &proof, int start) { return false; }
	string type;
	static void forward_and(vector<ProofLine> &proof) {
		vector<ProofLine> adding;
		bool find;
		for (auto p : proof) {
			for (auto q : proof) {
				TreeNode *and = new AndOpNode(p.formula, q.formula);
				find = false;
				for (auto r : proof) {
					if (r.formula->contains(and)) {
						find = true;
						ProofLine n;
						n.dependence.insert(p.dependence.begin(), p.dependence.end());
						n.dependence.insert(q.dependence.begin(), q.dependence.end());
						adding.push_back(n);
						break;
					}
				}
				if (!find) delete and;
			}
		}
		for (auto i : adding)
			proof.push_back(i);
	}
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
	bool pgen(set<int> &dep, vector<ProofLine> &proof, int start) {
		forward_and(proof);
		for (int i = start; i < proof.size(); i++) {
			if (proof[i].dependence.size() == 0 &&
				this->equals(proof[i].formula)) {
				return true;
			}
		}
		return false;
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
