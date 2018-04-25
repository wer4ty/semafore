#include <iostream>
#include <string>

#include <unistd.h>
#include <wait.h>
#include <errno.h>

#include <chrono>
#include <ctime>

#include "menuItem.h"
#include "Waiter.h"
#include "Customer.h"
#include "orderBoard.h"
#include "SharedMemoryWorker.h"

#include "semaphore_inc.h"


#define STATIC_ARRAY_SIZE 10

using namespace std;

// global params
int simulation_time, menu_items, customers, waiters;
menuItem menu[STATIC_ARRAY_SIZE];
orderBoard customers_orders[STATIC_ARRAY_SIZE];

// init timer
auto start = chrono::high_resolution_clock::now();
auto finish = chrono::high_resolution_clock::now();
auto microseconds = chrono::duration_cast<std::chrono::microseconds>(finish-start);
double seconds = 0.00;

// shared memory
string buffer;
char* shMemRes;
SharedMemoryWorker* menuSegment = new SharedMemoryWorker(rand() % 100000);
SharedMemoryWorker* orderSegment = new SharedMemoryWorker(rand() % 10000);

// semaphores
key_t mutex_key  = ftok(".", rand() % 1000);
int mutex; 

// key_t s1_key = ftok(".", rand() % 10000);
// int s1 = initsem(s1_key, 0);

// key_t s2_key = ftok(".", rand() % 100);
// int s2 = initsem(s1_key, customers); // number of possible orders by parallel 

// 0.Support functions
void update_timer() {
	auto finish = chrono::high_resolution_clock::now();
	microseconds = chrono::duration_cast<std::chrono::microseconds>(finish-start);
	seconds = (double) microseconds.count() / 1000000.0;
	cout << fixed << setprecision(3) << seconds << " ";
}

int sleep_time_gen(int min, int max) {
	srand ( time(NULL) );
	int len = (max - min) + 1;
	int *nums =  new int[len];

	int i = 0;
	while (i<len) {
		nums[i] = min;
		i++; min++;
	}
	int RIndex = rand() % len;
	int tmp = nums[RIndex];
	delete [] nums;
	return tmp;
}


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
	cout << "\n==========Menu list==========" << endl;
	cout << "Id " << left << setw(12) << "Name" << "Price " << "Orders" << endl;
	for(int i=0; i<number_items; i++) {
		menu[i].print();
	}
}


//4. create order boards for each future customer
void createOrdersBoards(int max) {
	if (max < 0) exit(1);
	for (int i=0; i<max; i++) {
		customers_orders[i] = {i, -1, 0}; // empty order board for each 
	}
}

// 5. Print order boards
void printOrderBoards(int number_items) {
	if (number_items < 1) exit(1);
	cout << "\n==========Orders list==========" << endl;
	cout << "cId " << left << setw(12) << "item id" << "Amount " << "Status" << endl;
	for(int i=0; i<number_items; i++) {
		customers_orders[i].print();
	}
}

// 6. Compress to string to save in shared memory
string compressToSegment(int flag) {
	// flag = 0 => menuItem
	// flag = 1 => orderBoard

	string tmp;
	if(flag) {
		for (int i=0; i<customers; i++) {
			tmp += customers_orders[i].str();
			tmp += ":";
		}	
	}

	else {
		for (int i=0; i<menu_items; i++) {
			tmp += menu[i].str();
			tmp += ":";
		}
	}
		tmp.pop_back();
		return tmp;
}

// 7. Put into shared memory 
void putInSharedMemory(int flag) {
	// flag = 0 => menuItem
	// flag = 1 => orderBoard

	if (flag == 0) {
	buffer =  compressToSegment(flag);
	buffer = buffer.erase(0,1);
	menuSegment->put(const_cast<char*>(buffer.c_str()));
	buffer.clear();
	}

	if (flag == 1) {
	buffer =  compressToSegment(flag);
	buffer = buffer.erase(0,1);
	orderSegment->put(const_cast<char*>(buffer.c_str()));
	buffer.clear();
	}
}

// 8. Decompress menu items from shared memory to global array menu[]
void menuDecompress() {
	char* tmp_menu = menuSegment->get(); // get from shared memory
	string s = tmp_menu;
	string delimiter = ":";
	int j = 0;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		menu[j].obj(const_cast<char*>(token.c_str()));
	    s.erase(0, pos + delimiter.length());
	    j++;
	}
	menu[j].obj(const_cast<char*>(s.c_str()));
	
}

// 9. Decompress order boards from shared memory to global array orderBoards[]
void orderBoardsDecompress() {
	
	char* tmp_orders = orderSegment->get(); // get from shared memory
	string s = tmp_orders;
	string delimiter = ":";
	int j = 0;
	size_t pos = 0;
	string token;
	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		customers_orders[j].obj(const_cast<char*>(token.c_str()));
	    s.erase(0, pos + delimiter.length());
	    j++;
	}
	customers_orders[j].obj(const_cast<char*>(s.c_str()));
}

// 10. Customers functions
void readMenu(int cId) {
	// check status of previous order
	if (customers_orders[cId].getOrderStatus()) { // true => finish 

	orderBoardsDecompress(); // get order from shared memory and transform to global array

	int RIndex = rand() % menu_items;
	int RAmount = rand() % 4;
	int probability = rand() % 2;

	if (RAmount == 0) { RAmount++; }

			// if probability 1 order
			if (probability) {
			cout << "\tCustomer ID: " << cId << " reads a menu about " << menu[RIndex].getName() <<" (ordered, amount " << RAmount << ")" << endl;
				customers_orders[cId].setItemId(RIndex);
				customers_orders[cId].setItemId(RAmount);
				customers_orders[cId].setOrderStatus(true);

				putInSharedMemory(1); // if make order put all orders in shared memory
			}
			// probability  0 do not order
			else {
			cout << "\tCustomer ID: " << cId << " reads a menu about " << menu[RIndex].getName() <<" (doesn't want to order)" << endl;
			}
	}
}


// 11. Waiters funtions
void performOrders(int wId) {

	menuDecompress();  // get menu from shared memory and transfrom to global array
	orderBoardsDecompress(); // get order from shared memory and transform to global array

	for (int i=0; i<customers; i++) {
		if (customers_orders[i].getOrderStatus() == false) {
			menu[customers_orders[i].getItemId()].increaseOrders();
			customers_orders[i].setOrderStatus(true);
			cout << "\tWaiter ID: " << wId << " performs the order of cutomer ID " << i << " ( " << customers_orders[i].getAmount() << " " << menu[customers_orders[i].getItemId()].getName() << ")";

			// make changes save it in shared memory
			putInSharedMemory(1);
			putInSharedMemory(0);
		}
		else {
			cout << "\tWaiter ID: " << wId << " no orders to perform " << endl;
		}
	}
}

// 12. Create sub-processes
void createSubProc() {
	int tmp_pid, tmp_status;
	
		// Create waiters 
		for (int i=0; i<waiters; i++) {
			tmp_pid = fork();
			if(tmp_pid == -1) { perror("Fork: "); exit(1); }
			
			else if(tmp_pid == 0) {
				update_timer();
				Waiter* w = new Waiter(i, getpid(), getppid());
				
				// waiter action
				while((int)seconds <= simulation_time) {

					//wait
					p(mutex);

				sleep(sleep_time_gen(1,2));
				update_timer();
				performOrders(w->getId());

					v(mutex);
					// leave
				}

				delete w;
				exit(0);	
			}
		}

		// Create customers 
		for (int i=0; i<customers; i++) {
			tmp_pid = fork();
			if(tmp_pid == -1) { perror("Fork: "); exit(1); }
			else if(tmp_pid == 0) {
				update_timer();
				Customer* c = new Customer(i, getpid(), getppid());

				// customer action
				while((int)seconds <= simulation_time) {
					//wait
					p(mutex);

				sleep(sleep_time_gen(3,6));
				update_timer();
				readMenu(c->getId());

					v(mutex);
					// leave
				}

				delete c;
				exit(0);	
			}
		}

		// main process wait while all children will stop
		while ((tmp_pid=waitpid(-1, &tmp_status, 0))!=-1) {}
}

 
int main(int argc, char* argv[]) {
	
	mutex = initsem(mutex_key, 1);

	checkInputParams(argc, argv);
	generateMenu(menu_items);
	printMenu(menu_items);
	createOrdersBoards(customers);

	// save into shared memory menu and empty order boards
	putInSharedMemory(0); // menu
	putInSharedMemory(1); // order boards for each customer


	//start multi-processing
	int pid,status;
	pid = fork();
	if (pid == -1) {
		perror("Fork: ");
		exit(1);
	}

	// child process (manager)
	else if (pid == 0) {		
		cout << "\n" << endl;
		update_timer();
		cout  << " Main process start creating sub-process" << endl;

		createSubProc(); // waiters and customers


	}

	// parent process
	else {
		wait(&status);

		update_timer();
		cout << " Main ID " << pid << " end work" << endl;
	}




	return 0;
}
