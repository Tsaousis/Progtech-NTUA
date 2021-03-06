// Polynomials.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
protected:
	class Term {
	public:
		int exponent, coefficient;
		Term *next;

		Term(int exp, int coeff, Term *n) {
			exponent = exp;
			coefficient = coeff;
			next = n;
		}
		int getExp() { return exponent; }
		int getCoeff() { return coefficient; }
		friend class Polynomial;
	};
	Term *head, *tail;

public:
	Polynomial() {
		head = tail = nullptr;
	}
	Polynomial(const Polynomial &p) {
		Term *t = p.head;
		head = tail = nullptr;

		if (t != nullptr) {
			head = tail = new Term(t->exponent, t->coefficient, nullptr);
			t = t->next;
		}
		while (t != nullptr) {
			tail->next = new Term(t->exponent, t->coefficient, nullptr);
			tail = tail->next;
			t = t->next;
		}
	}
	//~Polynomial();

	Polynomial & operator = (const Polynomial &p) {
		delete head;
		Term *t = p.head;
		head = tail = nullptr;

		if (t != nullptr) {
			head = tail = new Term(t->exponent, t->coefficient, nullptr);
			t = t->next;
		}
		while (t != nullptr) {
			tail->next = new Term(t->exponent, t->coefficient, nullptr);
			tail = tail->next;
			t = t->next;
		}
		return *this;
	}

	void addTerm(int expon, int coeff) {
		if (coeff == 0) return;
		if (head == nullptr) {
			head = tail = new Term(expon, coeff, nullptr);
			return;
		}

		Term *t = head->next;
		Term *q = head;

		if (expon > head->exponent) {
			head = new Term(expon, coeff, q);
			return;
		}
		if (expon == head->exponent) {
			head->coefficient += coeff;
			if (head->coefficient == 0) {
				head = head->next;
				delete q;                  //!!!!could be wrong 
			}
			return;
		}

		while (t != nullptr) {
			if (expon > t->exponent) {
				q->next = new Term(expon, coeff, t);
				return;
			}
			if (expon == t->exponent) {
				t->coefficient += coeff;
				if (t->coefficient == 0) {
					q->next = t->next;
					if (t->next == nullptr) tail = q;  // so as not to delete tail
					delete t;
				}
				return;
			}
			q = t;
			t = t->next;
		}
		tail->next = new Term(expon, coeff, nullptr);
		tail = tail->next;
	}
	double evaluate(double x) {
		if (head == nullptr) return 0;

		Term *t = head;
		double sum = 0;
		while (t != nullptr) {
			sum += t->coefficient * pow(x, t->exponent);
			t = t->next;
		}
		return sum;
	}

	friend Polynomial operator + (const Polynomial &p, const Polynomial &q) {
		Polynomial A;
		Term *t = p.head, *s = q.head;

		while (t != nullptr && s != nullptr) {
			if (t->exponent == s->exponent) {
				A.addTerm(t->exponent, t->coefficient + s->coefficient);
				t = t->next;
				s = s->next;
			}
			else if (t->exponent > s->exponent) {
				A.addTerm(t->exponent, t->coefficient);
				t = t->next;
			}
			else if (t->exponent < s->exponent) {
				A.addTerm(s->exponent, s->coefficient);
				s = s->next;
			}
		}

		while (t != nullptr) {
			A.addTerm(t->exponent, t->coefficient);
			t = t->next;
		}
		while (s != nullptr) {
			A.addTerm(s->exponent, s->coefficient);
			t = t->next;
		}
		return A;
	}
	friend Polynomial operator * (const Polynomial &p, const Polynomial &q) {
		Polynomial A;
		Term *t = p.head, *s = q.head;

		while (t != nullptr) {
			s = q.head;
			while (s != nullptr) {
				A.addTerm(t->exponent + s->exponent, t->coefficient * s->coefficient);
				s = s->next;
			}
			t = t->next;
		}

		return A;
	}

	friend ostream & operator << (ostream &out, const Polynomial &p) {
		if (p.head == nullptr) {
			out << "0";
			return out;
		}

		Term *t = p.head;
		if (t->coefficient > 0) {
			if (t->coefficient != 1) out << abs(t->coefficient);
			if (t->exponent != 0) {
				out << "x";
				if (t->exponent != 1) out << "^" << t->exponent;
			}
			else if (t->coefficient == 1 and t->exponent == 0) out << 1;
		}

		else if (t->coefficient < 0) {
			out << "- ";
			if (t->coefficient != -1) out << abs(t->coefficient);
			if (t->exponent != 0) {
				out << "x";
				if (t->exponent != 1) out << "^" << t->exponent;
			}
			else if (t->coefficient == -1 and t->exponent == 0) out << 1;
		}

		t = t->next;
		while (t != nullptr) {
			if (t->coefficient > 0) {
				out << " + ";
				if (t->coefficient != 1) out << abs(t->coefficient);
				if (t->exponent != 0) {
					out << "x";
					if (t->exponent != 1) out << "^" << t->exponent;
				}
				else if (t->coefficient == 1 and t->exponent == 0) out << 1;
			}
			
			else if (t->coefficient < 0) {
				out << " - ";
				if (t->coefficient != -1) out << abs(t->coefficient);
				if (t->exponent != 0) {
					out << "x";
					if (t->exponent != 1) out << "^" << t->exponent;
				}
				else if (t->coefficient == -1 and t->exponent == 0) out << 1;
			}
			t = t->next;
		}
		return out;
	}
};

int main()
{
	Polynomial a;
	a.addTerm(4, 1);
	a.addTerm(4, -1);
	a.addTerm(2, -100);
	cout << a;
	return 0;
}

