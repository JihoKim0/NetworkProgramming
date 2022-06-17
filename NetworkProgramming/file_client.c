//20220601
//file send program(client)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF 80

int main(int argc, char* argv[]) {
    int fd;
    int sd;

    char buf[BUFF];

    int len;
    struct sockaddr_in serv_addr;

    if (argc != 3) {
        printf("Usage: %s <O{> <port> \n", argv[0]);
        exit(1);
    }

    fd = open("rcv.dat", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0) {
        perror("file open error");
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    while ((len = read(sd, buf, BUFF)) != 0) {
        write(fd, buf, len);
    }

    write(sd, "Thanks\n", 10);

    close(fd);
    close(sd);

    return 0;
}