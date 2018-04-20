#include <iostream>

using namespace std;

class Waiter {
	private:
		int id;
		int pid;
		int ppid;

	public:
		Waiter();
		Waiter(int, int, int);
		~Waiter();

		inline int getId() { return id; }
};