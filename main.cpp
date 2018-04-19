#include <iostream>
#include <unistd.h>
#include <string>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>

#include "menuItem.h"
#include "Waiter.h"
#include "Customer.h"

using namespace std;

// global params
int simulation_time, menu_items, customers, waiters;
menuItem menu[10];

// 1. Check if all simulation params are correct
void checkInputParams(int argc, char* argv[]) {
	string error_args = "Input arguments are not valid";
	if (argc < 5)  { cout << error_args << endl;  exit(1); }

	simulation_time = atoi(argv[1]);
	menu_items = atoi(argv[2]);
	customers = atoi(argv[3]);
	waiters = atoi(argv[4]);

	if (  (simulation_time < 1 || simulation_time > 30)
	&& (menu_items < 1 || menu_items > 10)
	&& (customers < 1 || customers > 10)
	&& (waiters < 1 || waiters > 3) )
	{
		cout << error_args << endl;  exit(1);
	}

	else {
		cout << "\n=====Simulation arguments=====" << "\nSimulation time: " << simulation_time << "\nMenu items count: " << menu_items << "\nCustomers count: " << customers << "\nWaiters count: " << waiters << endl;
	}
}

//2. Generate menu
void generateMenu(int number_items) {
	string menuItemsNames[] = {"Pizza", "Salad", "Hamburger", "Spagetti", "Pie", "Milkshake", "Cake", "Soup", "Juice", "Tea"};
	int rand_price;
	if (number_items < 1) exit(1);
	for (int i=0; i<number_items; i++) {
		rand_price = (int) rand() % 100 + 1;
		menu[i] = {i+1, menuItemsNames[i], rand_price, 0};
	}
}

// 3. Print menu
void printMenu(int number_items) {
	if (number_items < 1) exit(1);
	//char* buffer;
	cout << "\n==========Menu list==========" << endl;
	cout << "Id " << left << setw(12) << "Name" << "Price " << "Orders" << endl;
	for(int i=0; i<number_items; i++) {
		menu[i].print();
		//buffer = menu[i].str();
	}
}

// 4 Create sub-processes
void createSubProc(int flag) {
	// flag 1 => Waiter
	// flag 0 => Customer

	int tmp_pid, tmp_status;

	if (flag) {
		// Create waiters 
		for (int i=0; i<waiters; i++) {
			tmp_pid = fork();
			if(tmp_pid == -1) { perror("Fork: "); exit(1); }
			else if(tmp_pid == 0) {
				Waiter* w = new Waiter(i, getpid(), getppid());
				delete w;
				exit(0);	
			}
			else {
					wait(&tmp_status);
			}

		}
	}

	else {
		// Create customers 
		for (int i=0; i<customers; i++) {
			tmp_pid = fork();
			if(tmp_pid == -1) { perror("Fork: "); exit(1); }
			else if(tmp_pid == 0) {
				Customer* c = new Customer(i, getpid(), getppid());
				delete c;
						exit(0);	
			}
			else {
					wait(&tmp_status);
			}

		}
	}
}
 
int main(int argc, char* argv[]) {
	int pid,status;
	

	checkInputParams(argc, argv);
	generateMenu(menu_items);
	printMenu(menu_items);

	pid = fork();
	if (pid == -1) {
		perror("Fork: ");
		exit(1);
	}

	// child process (manager)
	else if (pid == 0) {
		
		cout << "Main process start creating sub-process" << endl;

		createSubProc(1); // waiters
		createSubProc(0); // customers



		
	}

	// parent process
	else {
		wait(&status);
		cout << "Main ID " << pid << " end work" << endl;
	}




	return 0;
}
