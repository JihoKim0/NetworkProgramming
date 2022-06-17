//20220504
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    pid_t pid, cpid;
    int data = 10;
    int status;

    if ((pid = fork()) < 0) {
        perror("fork fail");
        exit(1);
    }

    printf("fork success: pid = %d\n", pid);

    if (pid == 0) { //child process
        data += 10;
    }
    else {                                //parent process
        data -= 10;
        cpid = wait(&status);
        printf("child pid = %d\n", cpid);
        printf("return val = %d\n", WEXITSTATUS(status));
        sleep(20);
    }
    
    printf("data: %d\n", data);

    return 0;
}