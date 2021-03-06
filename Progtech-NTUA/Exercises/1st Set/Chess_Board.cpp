// Chess_Board.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iomanip>
#include <stdexcept>
#include <iostream>
using namespace std;

class ChessBoardArray {
protected:
	class Row {
	private:
		ChessBoardArray *c;
		int row;

	public:
		Row(ChessBoardArray &a, int i) {
			c = &a;
			row = i;
		}
		int & operator [] (int i) const {
			return c->select(row, i);
		}
	};

	class ConstRow {
		const ChessBoardArray *c;
		int row;
	public:
		ConstRow(const ChessBoardArray &a, int i) {
			c = &a;
			row = i;
		}
		int operator [] (int i) const {
			return c->select(row, i);
		}
	};

	int n, b;
	int *data;	

	unsigned int loc(int i, int j) const throw(out_of_range) {
		int di = i - b, dj = j - b;
		
		if (di < 0 || di >= n) throw out_of_range ("out of array");
		if (dj < 0 || dj >= n) throw out_of_range ("out of array");
		if ((di + dj) % 2 == 1) throw out_of_range ("black square");

		//even side
		if(n % 2 == 0) return di * (n / 2) + dj / 2;

		//odd side (even di has n/2 + 1 whites and odd di has n/2 whites
		else return di * (n / 2) + di / 2 + di % 2 + dj / 2;
	}

public:
	ChessBoardArray(unsigned size = 0, unsigned base = 0) {
		n = size;
		b = base;
		data = new int[(n * n) / 2 + n % 2];

		for (int i = 0; i < (n * n) / 2 + n % 2 ; i++) data[i] = 0;
	}
	ChessBoardArray(const ChessBoardArray &a) {
		n = a.n;
		b = a.b;
		data = new int[(n * n) / 2 + n % 2];

		for (int i = 0; i < (n * n) / 2 + n % 2; i++) data[i] = a.data[i];
	}
	~ChessBoardArray() {
		delete[] data;
	}

	ChessBoardArray & operator = (const ChessBoardArray &a) {
		delete data;
		n = a.n;
		b = a.b;
		data = new int[(n * n) / 2 + n % 2];

		for (int i = 0; i <	(n * n) / 2 + n % 2; i++) data[i] = a.data[i];
		return *this;
	}

	int & select(int i, int j) {
		return data[loc(i, j)];
	}
	int select(int i, int j) const {
		return data[loc(i, j)];
	}

	const Row operator [] (int i) {
		return Row(*this, i);
	}
	const ConstRow operator [] (int i) const {
		return ConstRow(*this, i);
	}

	friend ostream & operator << (ostream &out, const ChessBoardArray &a) {
		int n;
		for (int i = a.b; i < a.n + a.b; i++) {	
			if ((i - a.b) % 2 == 1) {
				out << setw(4) << '0';
				n = a.b + 1;
			}
			else n = a.b;
			
			for (int j = n; j < a.n + a.b; j += 2) {
				out << setw(4) << a[i][j];
				if (j != a.n + a.b - 1) out << setw(4) << '0';
			}
			out << endl;
		}
		return out;
	}
};

int main()
{
	ChessBoardArray a(5, 1); // size 4x4, rows and columns numbered from 1
	a[3][1] = 42;
	a[4][4] = 17;
	try { a[2][1] = 7; }
	catch (out_of_range &e) { cout << "a[2][1] is black" << endl; }
	try { cout << a[1][2] << endl; }
	catch (out_of_range &e) { cout << "and so is a[1][2]" << endl; }
	cout << a;
	return 0;
}

