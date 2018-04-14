#include <iostream>
#include <unistd.h>
#include <string>
#include <wait.h>
#include <sys/types.h>

#include "menuItem.h"

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
		cout << "=====Simulation arguments=====" << "\nSimulation time: " << simulation_time << "\nMenu items count: " << menu_items << "\nCustomers count: " << customers << "\nWaiters count: " << waiters << endl;
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

	cout << "==========Menu list==========" << endl;
	cout << "Id " << left << setw(12) << "Name" << "Price " << "Orders" << endl;
	for(int i=0; i<number_items; i++) {
		menu[i].print();
	}
}
 
int main(int argc, char* argv[]) {
	
	

	checkInputParams(argc, argv);
	generateMenu(menu_items);
	printMenu(menu_items);


	return 0;
}
