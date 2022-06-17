//20220601
//the server can reuse address(test with echo_client.c)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[]) {
    int serv_sock;
    int clnt_sock;
    char msg[50];
    int str_len;
    int option;
    socklen_t optlen;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        perror("socket error");
        exit(1);
    }

    optlen = sizeof(option);
    option = TRUE;

    //      setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(serv_sock, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

    while (str_len = read(clnt_sock, msg, sizeof(msg)) > 0) {
        write(clnt_sock, msg, str_len);
        write(1, msg, str_len);
    }

    close(clnt_sock);
    return 0;
}