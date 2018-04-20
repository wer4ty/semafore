#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

class SharedMemoryWorker {
	private:
		char c;
		int shmid;
		key_t key;
		char *shm;
		char *s;
		int segment_size;

	public:
		SharedMemoryWorker();
		void put(char*);
		char* get();
		~SharedMemoryWorker();
};