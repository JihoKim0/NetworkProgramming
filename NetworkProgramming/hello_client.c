//20220413
//recieve message from TCP server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv) {
    int sock;
    char msg[30];
    int str_len;
    struct sockaddr_in server_addr;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // socket creation
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation error\n");
        exit(1);
    }

    // socket address initialization
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // connect to a server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("socket connect error\n");
        exit(1);
    }

    // receive data
    str_len = read(sock, msg, sizeof(msg) - 1);
    if (str_len < 0) {
        perror("data reception error\n");
        exit(1);
    }

    // process the received data
    msg[str_len] = 0;
    printf("Received message: %s\n", msg);

    // close socket
    close(sock);

    return 0;
}