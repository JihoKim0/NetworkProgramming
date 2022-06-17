//20220518
//IPC between two processes using shared memory(client)
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
    pData = ptr + sizeof(int);
    sprintf(pData, "This is a request from %d.", getpid());
    *pInt = 1;
    printf("Sent a request.....");

    while ((*pInt) == 1)
        ;

    printf("Received reply: %s\n", pData);
}