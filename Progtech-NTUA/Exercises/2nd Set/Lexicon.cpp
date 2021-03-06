// NTUA - ProgTech.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int string_compare(const string &a, const string &b) {
	int al = a.length();
	int bl = b.length();
	int minlength = min(al, bl);

	int i = 0;
	for (i; i < minlength; i++) {
		if (a[i] == b[i]) continue;
		else if (a[i] > b[i]) return 1;
		else if (a[i] < b[i]) return -1;
	}

	
	if (al == bl) return 0; //i == minlength
	else if (al > bl) return 1; //i == minlength
	else return -1; //i == minlength
}

class lexicon {
private:
	class node {
	public:
		string word;
		int freq;
		node *right, *left;
		node(const string &s, const int &f, node *r, node *l) {
			word = s;
			freq = f;
			right = r;
			left = l;
		}
	};
	node *root;
    
	int remove(const string &s) 
	{   //returns the frequency of the deleted word or 0 if it was not found
		if (root == nullptr) return 0;

		node *t = root, *last_t = t;
		int comparison, frequency = 0;
		
		while (t != nullptr) 
		{	//finding s
			comparison = string_compare(s, t->word);
			if (comparison == 1) {
				last_t = t;
				t = t->right;
			}
			else if (comparison == -1) {
				last_t = t;
				t = t->left;
			}
			else if (comparison == 0) break;
		} 

		if (t == nullptr) return 0;
		comparison = string_compare(t->word, last_t->word);

		if (t->left != nullptr && t->right != nullptr) {
			node *last_q = t->left, *q = last_q->right;			
			if (q == nullptr) {
				frequency = t->freq;				
				t->word = last_q->word;
				t->freq = last_q->freq;
				t->left = last_q->left;				
				delete last_q;
				return frequency;
			}

			while (q->right != nullptr) {
				last_q = q;
				q = q->right;
			}

			frequency = t->freq;
			t->word = q->word;
			t->freq = q->freq;
			last_q->right = q->left;			
			delete q;
			return frequency;
		}		
		else if (t->left == nullptr && t->right != nullptr) {
			if (comparison == 1) last_t->right = t->right;
			else if (comparison == -1) last_t->left = t->right;
			else root = t->right;

			frequency = t->freq;
			delete t;
			return frequency;
		}
		else if (t->left != nullptr && t->right == nullptr) {
			if (comparison == 1) last_t->right = t->left;
			else if (comparison == -1) last_t->left = t->left;
			else root = t->left;
			
			frequency = t->freq;
			delete t;
			return frequency;
		}		
		else {
			if (comparison == 1) last_t->right = nullptr;
			else if (comparison == -1) last_t->left = nullptr;
			else root = nullptr;
			
			frequency = t->freq;
			delete t;
			return frequency;
		}
	} 
	void print(ostream &out, node *p) const {
		 if (p != nullptr) {
			 print(out, p->left);
			 out << p->word << ' ' << p->freq << endl;
			 print(out, p->right);
		}
	}

public:
	lexicon() { root = nullptr; }

	void insert(const string &s, int k = 1) {
		if (root == nullptr) {
			root = new node(s, k, nullptr, nullptr);
			return;
		}
		
		node *q = root, *t = root;
		int comparison;
		
		while (t != nullptr) {
			comparison = string_compare(s, t->word);
			q = t;
			if (comparison == 1) {
				if (t->right == nullptr) {
					t->right = new node(s, k, nullptr, nullptr);
					return;
				}
				else t = t->right;
			}
			else if (comparison == -1) {
				if (t->left == nullptr) {
					t->left = new node(s, k, nullptr, nullptr);
					return;
				}
				else t = t->left;
			}
			else if (comparison == 0) {
					t->freq += k;
					return;
				}			
		}

		comparison = string_compare(s, q->word);
		if (comparison == 1) q->right = new node(s, k, nullptr, nullptr);
		else if (comparison == -1) q->left = new node(s, k, nullptr, nullptr);
	}
	int lookup(const string &s) const {
		if (root == nullptr) return 0;

		node *t = root;
		int comparison;
		while (t != nullptr) {
			comparison = string_compare(s, t->word);
			if (comparison == 1) t = t->right;
			if (comparison == -1) t = t->left;
			if (comparison == 0) return t->freq;
		}

		
		return 0; //t == nullptr
	}
	int depth(const string &s) {
		if (root == nullptr) return 0;

		node *t = root;
		int comparison, depth = 1;
		while (t != nullptr) {
			comparison = string_compare(s, t->word);
			if (comparison == 1) {
				t = t->right;
				depth++;
			}
			if (comparison == -1) {
				t = t->left;
				depth++;
			}
			if (comparison == 0) break;
		}

		if (t == nullptr) return 0;
		else return depth;
	}
	void replace(const string &s1, const string &s2) {
		int k = remove(s1);
		if (k == 0) return;
        
		else insert(s2, k);
	}

	friend ostream & operator << (ostream &out, const lexicon &l) {
		l.print(out, l.root);
		return out;
	}
};

int main() {
	lexicon a;
	a.insert("5");
	a.insert("9");
	a.insert("2");
	a.insert("4",10000);
	a.insert("3", 10);
	a.replace("5", "4");
	cout << a;
}

