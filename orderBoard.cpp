#include "orderBoard.h"

using namespace std;


orderBoard::orderBoard() {}

orderBoard::orderBoard(int c, int i, int a) {
	cId = c;
	itemId = i;
	amount = a;
	done = false;
}


orderBoard::~orderBoard() {}

void orderBoard::orderDone() {
	done = true;
}

void orderBoard::print() {
	cout << cId << " "<< itemId << " " << amount << " " << done << endl; 
}

char* orderBoard::str() {
	string tmp;
	tmp = to_string(cId)  
	+ "|" + to_string(itemId)  
	+ "|" + to_string(amount)  
	+ "|" + to_string(done)
	;

	cout << tmp << endl;

	return const_cast<char*>(tmp.c_str());
}