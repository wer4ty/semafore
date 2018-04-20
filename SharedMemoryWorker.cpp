#include "SharedMemoryWorker.h"

using namespace std;


SharedMemoryWorker::SharedMemoryWorker() {
	srand ( time(NULL) );
	int randNum =  rand() % 100000;
	key = ftok(".", randNum);
	segment_size = 1024;

}


		void SharedMemoryWorker::put(char* buf) {

			//Create the segment.
			if ((shmid = shmget(key, segment_size, IPC_CREAT | 0666)) < 0) {
				perror("shmget");
				exit(1);
			}

			// Attach segment
			if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1) {
				perror("shmat");
				exit(1);
			}
			s = shm;

			for (int i=0; buf[i] != '\0'; i++)
				*s++ = buf[i];
			*s = '\0';
		}


		char* SharedMemoryWorker::get() {

			// Locate the segment.
			if ((shmid = shmget(key, segment_size, 0666)) < 0) {
				perror("shmget");
				exit(1);
			}

			// Attach segment
			if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1) {
				perror("shmat");
				exit(1);
			}

			return shm;
		}

SharedMemoryWorker::~SharedMemoryWorker() {}