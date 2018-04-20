#include "Customer.h"

using namespace std;


Customer::Customer() {}

Customer::Customer(int i, int p, int pp) {
	id = i;
	pid = p;
	ppid = pp;

	cout << "\tCustomer " << id << " created  PID " << pid << " PPID " << ppid << endl;

}


Customer::~Customer() {
	cout << "\tCustomer " << id << " PID end work " << pid << " PPID " << ppid << endl;
}