#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <wait.h>


#define SEMPERM 0666

int initsem(key_t semkey, int initval)
{
	int status = 0, semid;

	union semun 
	{
		int val;
		struct semid_ds *stat;
		ushort *array;
	} ctl_arg;

	if ( ( semid = semget(semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL) ) == -1 )
	{		
			if ( (semid = semget(semkey, 1, 0) ) == -1)
			{
				perror("semget: semget failed");
				exit(0);			
			}
	}
		
	ctl_arg.val = initval;


	status = semctl(semid, 0, SETVAL, ctl_arg);

	if(status == -1)
	{
		perror("initsem failed");
		exit(0);
	}
	
	return semid;	
}

void p(int semid)
{
	struct sembuf p_buf;
	p_buf.sem_num = 0;
	p_buf.sem_op = -1;
	p_buf.sem_flg = SEM_UNDO;

	if( semop( semid, &p_buf, 1) == -1 )
	{
		perror("p(semid) failed");
		exit(0);
	}
}

void v(int semid)
{
	struct sembuf v_buf;
	v_buf.sem_num = 0;
	v_buf.sem_op = 1;
	v_buf.sem_flg = SEM_UNDO;

	if( semop( semid, &v_buf, 1 ) == -1 )
	{
		perror("v(semid) failed");
		exit(0);
	}
} 