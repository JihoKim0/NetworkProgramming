//202220518
//synchronization between parent and child using pipe
#include <stdio.h>
#include <stdlib.h>
#include "synclib.h"

#define NLOOPS  5


void main()
{
    int             i;
    pid_t   pid;

    TELL_WAIT();
    if ((pid = fork()) < 0) {      //create process & 오류 처리
        perror("fork");
        exit(1);
    }
    else if (pid > 0) {    //parent process
        for (i = 0; i < NLOOPS; i++) {
            TELL_CHILD();   //tell child process
            printf("Parent: Tell to child\n");
            printf("Parent: Wait for child to tell\n");
            WAIT_CHILD();   //wait child process
        }
    }
    else { //child process
        for (i = 0; i < NLOOPS; i++) {
            printf("Child: Wait for parent to tell\n");
            WAIT_PARENT();  //wait parent process
            TELL_PARENT();  //tell parent prcess
            printf("Child: Tell to parent\n");
        }
    }
}