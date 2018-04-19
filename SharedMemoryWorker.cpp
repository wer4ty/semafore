#include "SharedMemoryWorker.h"

using namespace std;


SharedMemoryWorker::SharedMemoryWorker() {
	key = ftok(".", 'S');
	segment_size = 1024;
}


		bool put();
		bool get();
		key_t returnKey();

SharedMemoryWorker::~SharedMemoryWorker() {}