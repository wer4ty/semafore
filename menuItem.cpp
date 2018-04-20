#include "menuItem.h"

using namespace std;


menuItem::menuItem() {}

menuItem::menuItem(int i, string n, int p, int o) {
	id = i;
	name = n;
	price = p;
	orders = o;
}


menuItem::~menuItem() {}

void menuItem::print() {
	cout << id << " " << left  << setw(15)<< name << setw(5) << price << orders << endl; 
}

char* menuItem::str() {
	string tmp;
	tmp += to_string(id)  
	+ "|" + name  
	+ "|" + to_string(price)  
	+ "|" + to_string(orders)
	;

	return const_cast<char*>(tmp.c_str());
}

menuItem* menuItem::obj(char * str) {
	string s = str;

	std::string delimiter = "|";
	int i = 0;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
	    token = s.substr(0, pos);
	    s.erase(0, pos + delimiter.length());

	    if (i == 0) { if(token.empty()) {id = 1; } else { id = stoi(token); } }
	    if (i == 1) { name = token; }
	    if (i == 2) { price = stoi(token); }
	    i++;
	}
	
	orders = stoi(s);
	return this;
}