//20220413
//send domain name to server & recieve host name from server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUFF 1024

int main(int argc, char** argv) {
    int sock;
    char msg[BUFF];
    int str_len;
    struct sockaddr_in serv_addr;
    struct hostent* host;
    if (argc != 3) {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation error");
        exit(1);

    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect error");
        exit(1);
    }

    while (1) {
        fputs("input domain name (q to quit): ", stdout);
        fgets(msg, BUFF, stdin);

        if (!strcmp(msg, "q\n")) {
            break;
        }
        write(sock, msg, strlen(msg));

        str_len = read(sock, msg, BUFF - 1);
        msg[str_len] = 0;
        msg[strlen(msg) - 1] = '\0';
        host = gethostbyname(msg);
        if (!host) {
            exit(1);
        }
        printf("hostname: %s \n", host->h_name);
    }

    close(sock);

    return 0;
}