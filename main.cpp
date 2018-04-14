#include <iostream>
#include <unistd.h>
#include <string>
#include <wait.h>
#include <sys/types.h>

#include <cstring>


using namespace std;



int main(int argc, char* argv[]) {
	
	string error_args = "Input arguments are not valid";
	if (argc < 5)  { cout << error_args << endl;  exit(1); }

	int simulation_time = atoi(argv[1]);
	int menu_items = atoi(argv[2]);
	int customers = atoi(argv[3]);
	int waiters = atoi(argv[4]);

	if (  (simulation_time < 1 || simulation_time > 30)
	&& (menu_items < 1 || menu_items > 10)
	&& (customers < 1 || customers > 10)
	&& (waiters < 1 || waiters > 3) )
	{
		cout << error_args << endl;  exit(1);
	}

	else {
		cout << "=====Simulation arguments=====" << "\nSimulation time: " << simulation_time << "\nMenu items count: " << menu_items << "\nCustomers count: " << customers << "\nWaiters count: " << waiters << endl;
	}



	return 0;
}
