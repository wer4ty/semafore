#include "orderBoard.h"

using namespace std;


orderBoard::orderBoard() {}

orderBoard::orderBoard(int c, int i, int a) {
	cId = c;
	itemId = i;
	amount = a;
	done = true;
}


orderBoard::~orderBoard() {}

void orderBoard::print() {
	cout << cId << " "<< itemId << " " << amount << " " << done << endl; 
}

char* orderBoard::str() {
	string tmp;
	tmp += to_string(cId)  
	+ "|" + to_string(itemId)  
	+ "|" + to_string(amount)  
	+ "|" + to_string(done)
	;

	return const_cast<char*>(tmp.c_str());
}

orderBoard* orderBoard::obj(char * str) {
	string s = str;

	std::string delimiter = "|";
	int i = 0;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
	    token = s.substr(0, pos);
	    s.erase(0, pos + delimiter.length());

	    if (i == 0) { if(token.empty()) { cId = 0; } else { cId = stoi(token); }  }
	    if (i == 1) { itemId = stoi(token); }
	    if (i == 2) { amount = stoi(token); }
	    i++;
	}
	
	int tmp_d = stoi(s);
	if (tmp_d) done = true;
	else done = false;

	return this;
}