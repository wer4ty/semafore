#include <iostream>

using namespace std;

class Customer {
	private:
		int id;
		int pid;
		int ppid;

	public:
		Customer();
		Customer(int, int, int);
		~Customer();
		void run();
};