#include "SyntaxTree.h"

string ProofLine::toString(int i) {
	stringstream ss;
	ss << std::left << setw(15) << TreeNode::set_to_string(dep);
	ss << setw(8) << std::left << "(" + to_string(i) + ")";
	ss << setw(40) << std::left << formula->toString();
	ss << setw(20)<< std::left << TreeNode::vector_to_string(args) + " " + tag;
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
	static vector<string> thinking_stack;
	string pgs = pgen_arg_str(dep, proof);
	if (std::find(thinking_stack.begin(), thinking_stack.end(), pgs) != thinking_stack.end()) {
		return -1;
	}
	cout << "proving " << toString() << endl;
	thinking_stack.push_back(pgs);
	int i = find(dep, this, proof), j;
	int result = -1;
	if (i != -1) {
		thinking_stack.pop_back();
		return i;
	}
	TreeNode *any = new AnyNode();
	TreeNode *and_left = new AndOpNode(this, any);
	TreeNode *and_right = new AndOpNode(any, this);
	TreeNode *if_node = new IfOpNode(any, this);
	TreeNode *not_node = new NotNode(this);
	TreeNode *dnot_node = new NotNode(not_node);

	vector<TreeNode *> and_left_matches, and_right_matches;
	vector<TreeNode *> if_matches, dnot_matches;
	if (and_left->helpful(proof, and_left_matches)) {
		for (auto m : and_left_matches) {
			if ((i = m->pgen(dep, proof)) != -1) {
				ProofLine p(proof[i].dep, this, "^E");
				p.arg(i);
				proof.push_back(p);
				result = proof.size() - 1;
				goto end;
			}
		}
	}
	if (and_right->helpful(proof, and_right_matches)) {
		for (auto m : and_right_matches) {
			if ((i = m->pgen(dep, proof)) != -1) {
				ProofLine p(proof[i].dep, this, "^E");
				p.arg(i);
				proof.push_back(p);
				result = proof.size() - 1;
				goto end;
			}
		}
	}

	if (if_node->helpful(proof, if_matches)) {
		for (auto m : if_matches) {
			if ((i = m->pgen(dep, proof)) != -1) {
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
	}

	
	if (dnot_node->helpful(proof, dnot_matches)) {
		TreeNode *m = dnot_matches[0];
		if ((i = m->pgen(dep, proof)) != -1) {
			ProofLine p(proof[i].dep, this, "DN");
			p.arg(i);
			proof.push_back(p);
			result = proof.size() - 1;
			goto end;
		}
	}

	//if(!neg_asp_lock)
	//	pgen_neg_asp(dep, proof);


end:
	thinking_stack.pop_back();
	delete and_left, and_right, if_node;
	delete not_node, dnot_node;
	//delete any;
	return result;
}

int TreeNode::pgen_neg_asp(set<int> dep, vector<ProofLine>& proof)
{
	Proof proof_copy = proof;
	set<int> d; d.insert(proof.size());
	int i = proof.size();
	TreeNode *n = type == "NotNode" ? dynamic_cast<NotNode *>(this)->child : new NotNode(this);
	n->neg_asp_lock = true;
	cout << "asuming " << n->toString() << endl;
	ProofLine asp(d, n, "ASP!");
	proof.push_back(asp);
	TreeNode *ctd = new CtdNode();
	int j = ctd->pgen(union_set(dep, d), proof);
	if (j != -1) {
		TreeNode *dn = new NotNode(n);
		ProofLine p(dep, dn, "!I");
		p.arg(i).arg(j);
		proof.push_back(p);
		return proof.size() - 1;
	}
	proof = proof_copy;
	return -1;
}

int CtdNode::pgen(set<int> dep, vector<ProofLine> &proof) {
	int i, j, k;
	if ((k = this->TreeNode::pgen(dep, proof)) != -1) {
		return k;
	}
	Proof proof_copy = proof;
	for (i = 0; i < proof.size(); i++) {
		TreeNode *np = new NotNode(proof[i].formula);
		k = np->pgen(dep, proof);
		if (k != -1) {
			ProofLine pline(dep, this, "!E");
			pline.arg(i).arg(k);
			proof.push_back(pline);
			return proof.size() - 1;
		}
		delete np;
	}
	proof = proof_copy;
	return -1;
}
