#include "SyntaxTree.h"

string ProofLine::toString(int i) {
	stringstream ss;
	ss << "{" << TreeNode::set_to_string(dep);
	ss << "} (" << i << ")\t\t";
	ss << formula->toString() << "\t\t" << TreeNode::vector_to_string(args) << tag;
	return ss.str();
}
void ProofLine::printProof(Proof &p) {
	cout << "-----proof-----" << endl;
	for (int i = 0; i < p.size(); i++) {
		cout << p[i].toString(i) << endl;
	}
	cout << endl;
}

int TreeNode::find(set<int> dep_most, TreeNode *formula, vector<ProofLine> proof) {
	for (int i = 0; i < proof.size(); i++) {
		if (proof[i].formula->equals(formula) && is_subset(proof[i].dep, dep_most)) {
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

set<int> TreeNode::union_set(set<int>& a, set<int>& b)
{
	set<int> c(begin(a), end(a));
	c.insert(begin(b), end(b));
	return c;
}

set<int> TreeNode::substract_set(set<int>& a, set<int>& b)
{
	set<int> c(begin(a), end(a));
	for (auto i : b) {
		c.erase(i);
	}
	return c;
}

string TreeNode::set_to_string(set<int>& a)
{
	stringstream ss;
	for (auto i : a) {
		ss << i << ",";
	}
	string s = ss.str();
	if (s.size() > 0) {
		s = s.substr(0, s.size() - 1);
	}
	return s;
}
string TreeNode::vector_to_string(vector<int>& a)
{
	stringstream ss;
	for (auto i : a) {
		ss << i << ",";
	}
	string s = ss.str();
	if (s.size() > 0) {
		s = s.substr(0, s.size() - 1);
	}
	return s;
}

int TreeNode::pgen(set<int> dep, vector<ProofLine> &proof) {
	cout << "proving " << toString() << endl;
	int i = find(dep, this, proof), j;
	int result = -1;
	if (i != -1) {
		return i;
	}
	TreeNode *any = new AnyNode();

	TreeNode *and_left = new AndOpNode(this, any);
	TreeNode *and_right = new AndOpNode(any, this);
	if (and_left->helpful(proof) && (i = and_left->pgen(dep, proof)) != -1 ||
		and_right->helpful(proof) && (i = and_right->pgen(dep, proof)) != -1 ) {
		ProofLine p(proof[i].dep,this, "^E");
		p.arg(i);
		proof.push_back(p);
		result = proof.size() - 1;
		goto end;
	}
	

	TreeNode *if_node = new IfOpNode(any, this);
	TreeNode *if_match;
	if (if_node->helpful(proof, &if_match)) {
		i = if_match->pgen(dep, proof);
		if (i != -1) {
			IfOpNode *cond = dynamic_cast<IfOpNode *>(proof[i].formula);
			if ((j = cond->left->pgen(dep, proof)) != -1) {
				ProofLine p(union_set(proof[i].dep, proof[j].dep), this, "->E");
				p.arg(i).arg(j);
				proof.push_back(p);
				result = proof.size() - 1;;
				goto end;
			}
		}
	}
	

end:
	delete and_left, and_right, if_node;
	delete any;
	return result;
}