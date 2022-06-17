//20220323 open the file
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, char* argv[]) {
    int fd;
    char buf[MAX_BUF];

    if (argc != 2) {
        printf("Usage: %s path\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_APPEND)) < 0) {
        perror("no file exist");
        exit(1);
    }

    printf("success: %s\n", argv[1]);

    close(fd);

    return 0;
}