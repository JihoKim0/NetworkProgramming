//20220601
//file send program(server)
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

    int serv_sd;
    int clnt_sd;
    int fd;
    char buf[BUFF];

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;
    int len;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    fd = open("file_server.c", O_RDONLY);
    if (fd < 0) {
        perror("file open error");
        exit(1);
    }

    serv_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sd < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(serv_sd, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sd < 0) {
        perror("accept error");
        exit(1);
    }

    while ((len = read(fd, buf, BUFF)) > 0) {
        write(clnt_sd, buf, len);
    }

    if (shutdown(clnt_sd, SHUT_WR) < 0) {
        perror("shutdown error");
        exit(1);
    }

    len = read(clnt_sd, buf, BUFF);
    write(1, buf, len);

    close(fd);
    close(clnt_sd);

    return 0;
}