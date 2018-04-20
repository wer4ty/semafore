#include <iostream>

using namespace std;

class orderBoard {
	private:
		int cId;
		int itemId;
		int amount;
		bool done;

	public:
		orderBoard();
		orderBoard(int, int, int);
		void orderDone();
		~orderBoard();
		void print();
		char* str();
		orderBoard* obj(char *);

		inline int getId() {return cId; }
		inline int getItemId() {return itemId; }
		inline int getAmount() {return amount; }
		inline bool getOrderStatus() {return done;}

		inline void setItemId(int item) {itemId = item; }
		inline void setAmount(int a) {amount = a; }
		inline void setOrderStatus(bool status) {done = status; }
};