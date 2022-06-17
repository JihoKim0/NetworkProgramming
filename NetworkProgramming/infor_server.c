//20220427
//provide daytime service & host information(UDO server program)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#define BUF 100

int main(int argc, char** argv) {
    int serv_sock;
    char msg[BUF];
    int str_len;
    time_t tim;
    tim = time(NULL);

    struct hostent* host;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }
    char* ptr;
    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, msg, BUF, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        ptr = strtok(msg, "\n");
        strcpy(msg, ptr);
        if (!strcmp(msg, "time")) {
            strcpy(msg, "Today is ");
            strcat(msg, ctime(&tim));
        }
        else {
            host = gethostbyaddr(msg, 4, AF_INET);
            if (!host) {
                printf("input error\n");
                exit(1);
            }
            strcpy(msg, "Host name is ");
            strcat(msg, host->h_name);

        }
        sendto(serv_sock, msg, str_len, 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
    }

    close(serv_sock);

    return 0;
}