//20220427
//UDP client with boundary
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
    int i;

    char msg1[] = "Good ";
    char msg2[] = "Afternoon ";
    char msg3[] = "Everybody.";

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

    sendto(sock, msg1, strlen(msg1), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg2, strlen(msg2), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    sendto(sock, msg3, strlen(msg3), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    for (i = 0; i < 3; i++) {
        addr_size = sizeof(from_addr);
        str_len = recvfrom(sock, msg, BUF, 0, (struct sockaddr*)&from_addr, &addr_size);
        msg[str_len] = 0;
        printf(" a message from a server (#%d): %s\n", i, msg);
    }

    close(sock);

    return 0;
}