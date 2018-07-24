#include "SyntaxTree.h"

int TreeNode::find(set<int> dep_most, TreeNode *formula, vector<ProofLine> proof) {
	for (int i = 0; i < proof.size(); i++) {
		if (proof[i].formula->equals(formula) && is_subset(proof[i].dependence, dep_most)) {
			return i;
		}
	}
	return -1;
}

bool TreeNode::is_subset(set<int> &a, set<int> &b)
{
	for (auto i : a) {
		if (b.find(i) == b.end())
			return false;
	}
	return true;
}

set<int> TreeNode::set_union(set<int>& a, set<int>& b)
{
	set<int> c(begin(a), end(a));
	c.insert(begin(b), end(b));
	return c;
}

bool TreeNode::pgen(set<int> &dep, vector<ProofLine> &proof, int start) {
	int i = find(dep, this, proof);
	if (i != -1) {
		return true;
	}
	TreeNode *any = new AnyNode();
	TreeNode *and_left = new AndOpNode(this, any);
	TreeNode *and_right = new AndOpNode(any, this);
	if (and_left->helpful(proof) && and_left->pgen(dep, proof, start) ||
		and_right->helpful(proof) && and_right->pgen(dep, proof, start)) {
		return true;
	}
	delete and_left, and_right;
	delete any;
	return false;
}