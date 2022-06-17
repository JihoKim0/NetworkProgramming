//20220323 read the file
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, char* argv[]) {
    int fd, count;
    char buf[MAX_BUF];

    if (argc != 2) {
        printf("Usage: %s path\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    printf("success: %s\n", argv[1]);

    while ((count = read(fd, buf, MAX_BUF)) > 0) {
        printf("%s\n", buf);
    }

    close(fd);

    return 0;
}
