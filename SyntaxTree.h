#pragma once
#include "utils.h"
class TreeNode;
class AtomNode;
class AnyNode;
class CtdNode;
class UniOpNode;
class NotNode;
class BiOpNode;
class AndOpNode;
class OrOpNode;
class IfOpNode;
class EqOpNode;
struct ProofLine;
typedef vector<ProofLine> Proof;
struct ProofLine {
	ProofLine(set<int> dep, TreeNode *fml, string tag) {
		this->dep = dep;
		this->formula = fml;
		this->tag = tag;
	}
	set<int> dep;
	//int lineNum;
	TreeNode *formula;
	vector<int> args;
	string tag;
	ProofLine &arg(int i) { args.push_back(i); return *this; }
	string toString(int i);
	static void printProof(Proof &p);
	
};
class TreeNode {
public:
	virtual string toString() = 0;
	virtual bool eval() { return false; }
	virtual void assign(string symbol, bool value) {}
	virtual void all_symbols(set<string> &symbols) {}
	virtual bool equals(TreeNode *tree) { return (tree->type == "AnyNode" || type == tree->type); }
	virtual bool contains(TreeNode *tree) { return type == tree->type; }
	virtual int pgen(set<int> dep, vector<ProofLine> &proof);
	bool helpful(vector<ProofLine> &proof) {
		for (auto p : proof) {
			if (p.formula->contains(this))
				return true;
		}
		return false;
	}
	string type;
	static int find(set<int> dep_most, TreeNode *formula, vector<ProofLine> proof);
	static bool is_subset(set<int> &a, set<int> &b);
	static set<int> union_set(set<int> &a, set<int> &b);
	static set<int> substract_set(set<int> &a, set<int> &b);
	static string set_to_string(set<int> &a);
	static string vector_to_string(vector<int>& a);
	
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
		if (tree->type == "AnyNode")
			return true;
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
	string symbol;
	bool value;
};

class AnyNode : public TreeNode {
public:
	AnyNode() :TreeNode() {
		this->type = "AnyNode";
	}
	string toString() {
		return "*";
	}
	bool equals(TreeNode *tree) {
		return true;
	}
	bool contains(TreeNode *tree) {
		return true;
	}
};

class CtdNode : public TreeNode {
public:
	CtdNode() : TreeNode() {
		this->type = "CtdNode";
	}
	string toString() {
		return "/\\";
	}
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
		if (tree->type == "AnyNode")
			return true;
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
		if (tree->type == "AnyNode")
			return true;
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
	int pgen(set<int> dep, vector<ProofLine> &proof) {
		int k;
		if ((k = this->TreeNode::pgen(dep, proof)) != -1) {
			return k;
		}
		Proof proof_copy = proof;
		int i = left->pgen(dep, proof);
		int j = right->pgen(dep, proof);
		if (i != -1 && j != -1) {
			ProofLine p(union_set(proof[i].dep, proof[j].dep), this, "^I");
			p.arg(i).arg(j);
			proof.push_back(p);
			return proof.size() - 1;
		}
		proof = proof_copy;
		return -1;
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
	int pgen(set<int> dep, Proof &proof) {
		int k;
		if ((k = this->TreeNode::pgen(dep, proof)) != -1) {
			return k;
		}
		Proof proof_copy = proof;
		set<int> d;
		d.insert(proof.size());
		ProofLine asp(d, left, "ASP->");
		proof.push_back(asp);
		int i = proof.size() - 1, j;
		if ((j = right->pgen(union_set(dep, d), proof)) != -1) {
			ProofLine p(substract_set(proof[j].dep, d), this, "->I");
			p.arg(i).arg(j);
			proof.push_back(p);
			return proof.size() - 1;
		}
		proof = proof_copy;
		return -1;
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
