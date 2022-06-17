//20220323 copy file
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, char* argv[]) {
    int fd1, fd2, count;
    char buf[MAX_BUF];

    if (argc != 3) {
        printf("wrong message\n");
        exit(1);
    }

    if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
        perror("no file");
        exit(1);
    }

    fd2 = open(argv[2], O_CREAT | O_WRONLY, 00700);

    while ((count = read(fd1, buf, MAX_BUF)) > 0) {
        write(fd2, buf, count);
    }

    close(fd1);
    close(fd2);
    return 0;
}