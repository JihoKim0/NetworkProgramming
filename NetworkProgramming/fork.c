//20220504
//practice multi process with fork
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    pid_t pid;
    int data = 10;

    if ((pid = fork()) < 0) {
        perror("fork fail");
        exit(1);
    }

    printf("fork success, process id: %d\n", pid);

    if (pid == 0) { // child process
        data += 10;
    }
    else {                // parent process
        data -= 10;
    }

    printf("data : %d \n", data);

    return 0;

}