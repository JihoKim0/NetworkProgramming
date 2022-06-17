//20220427
//provide daytime service & host information(UDO client program)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF 100

int main(int argc, char** argv) {
    int sock;
    char msg[BUF];
    int str_len;
    int addr_size;

    struct sockaddr_in serv_addr;
    struct sockaddr_in from_addr;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket error");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    while (1) {
        fputs("Input: ", stdout);
        fgets(msg, sizeof(msg), stdin);
        if (!strcmp(msg, "q\n")) {
            break;
        }

        sendto(sock, msg, BUF, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        addr_size = sizeof(from_addr);
        str_len = recvfrom(sock, msg, BUF, 0, (struct sockaddr*)&from_addr, &addr_size);

        printf("=> %s\n", msg);
    }

    close(sock);


    return 0;
}