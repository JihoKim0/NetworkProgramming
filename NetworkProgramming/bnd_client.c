//20220413
//TCP client with no bondary
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv) {
    int sock;
    int str_len, i;
    struct sockaddr_in serv_addr;

    char msg1[] = "Hello ";
    char msg2[] = "My name is Gilldong";
    char msg3[10];

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

    write(sock, msg1, strlen(msg1));
    write(sock, msg2, strlen(msg2));
    sleep(10);

    for (i = 0; i < 4; i++) {
        str_len = read(sock, msg3, sizeof(msg3) - 1);
        msg3[str_len] = 0;
        printf("Received message: %s \n", msg3);
    }

    close(sock);

    return 0;
}