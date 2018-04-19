#include "Waiter.h"

using namespace std;


Waiter::Waiter() {}

Waiter::Waiter(int i, int p, int pp) {
	id = i;
	pid = p;
	ppid = pp;

	cout << "\tWaiter " << id << " created  PID " << pid << " PPID " << ppid << endl;

}


Waiter::~Waiter() {
	cout << "\tWaiter " << id << " PID end work " << pid << " PPID " << ppid << endl;
}