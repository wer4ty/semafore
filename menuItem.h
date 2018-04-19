#include <iostream>
#include <string>

#include <iomanip>
#include <limits>
using namespace std;

class menuItem {
	private:
		int id;
		string name;
		int price;
		int orders;

	public:
		menuItem();
		menuItem(int, string, int, int);
		~menuItem();
		void print();
		char* str();
};