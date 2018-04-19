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
};