//20220413
//develop echo client(add recieve exception process)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF 1024

int main(int argc, char** argv) {
    int sock;
    char msg[BUFF];
    int str_len, recv_len, recv_num;
    struct sockaddr_in serv_addr;

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
        fputs("input message (q to quit): ", stdout);
        fgets(msg, BUFF, stdin);

        if (!strcmp(msg, "q\n")) {
            break;
        }
        str_len = write(sock, msg, strlen(msg));

        for (recv_len = 0; recv_len < str_len; ) {

            recv_num = read(sock, &msg[recv_len], str_len - recv_len);
            if (recv_num < 0) {
                perror("receive error");
                exit(1);
            }
            recv_len += recv_num;
        }

        msg[str_len] = 0;
        printf("Message from a server: %s \n", msg);
    }

    close(sock);

    return 0;
}