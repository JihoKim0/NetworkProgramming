//20220504
//TCP multi echo client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF 100

int main(int argc, char** argv) {
    struct sockaddr_in serv_addr;
    int sock;
    int n;
    pid_t pid;
    char msg[BUFF];

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    //소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() error");
        exit(1);
    }
    //소켓 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //서버에 연결
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect() error");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("fork() error");
        exit(1);
    }

    else if (pid > 0) {    //parent process
        while (fgets(msg, sizeof(msg), stdin) != 0) {
            write(sock, msg, strlen(msg));

            if (strstr(msg, "exit") != 0) {
                puts("Good bye!");
                close(sock);
                exit(0);
            }
        }
    }

    else {                                  //child process
        while (1) {
            if (n = read(sock, msg, BUFF) < 0) {
                perror("read() error");
                close(sock);
                exit(0);
            }
            msg[n] = 0;

            if (strstr(msg, "exit") != 0) {
                break;
            }

            printf("%s", msg);
        }
    }

    close(sock);

    return 0;
}