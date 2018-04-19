#include <cstdlib>
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
		int segment_size;

	public:
		SharedMemoryWorker();
		bool put();
		bool get();
		key_t returnKey();
		~SharedMemoryWorker();
};