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
		menuItem* obj(char *);

		inline void increaseOrders(int num) { orders+=num; }
		inline string getName() {return name; }
		inline int getId() {return id; }
		inline int getPrice() {return price; }
		inline int getOrdersNum() {return orders; }
};