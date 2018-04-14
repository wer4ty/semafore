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

