//20220518
//IPC between two processes using shared memory(server)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


void main()
{
    int             shmid;
    char* ptr, * pData;
    int* pInt;


    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {       //obtain shared memory identifier
        perror("shmget");
        exit(1);
    }
    if ((ptr = shmat(shmid, 0, 0)) == (void*)-1) {       //attach shared memory segment to process address space
        perror("shmat");
        exit(1);
    }

    pInt = (int*)ptr;
    while ((*pInt) == 0)
        ;

    pData = ptr + sizeof(int);
    printf("Received request: %s.....", pData);
    sprintf(pData, "This is a reply from %d.", getpid());
    *pInt = 0;
    printf("Replied.\n");

    sleep(1);       //sleep 1 seconds

    if (shmctl(shmid, IPC_RMID, 0) < 0) {  //identify shared memory
        perror("shmctl");
        exit(1);
    }
}