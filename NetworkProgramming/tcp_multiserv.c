//20220504
//TCP multi echo server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUFF 100

void processHandler(int sig);   //좀비프로세스 방지


int main(int argc, char** argv) {
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;

    struct sigaction act;
    int addr_size, str_len, state;
    pid_t pid;
    char msg[BUFF];

    if (argc != 2) {        //입력 오류
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    //좀비 프로세스 방지
    act.sa_handler = processHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    state = sigaction(SIGCHLD, &act, 0);    // register signal handler
    if (state < 0) {
        perror("sigaction error");
        exit(1);
    }

    //소켓생성 및 설정(초기화)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    //소켓 결합
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }
    //클라이언트 요청 대기
    if (listen(serv_sock, 5) < 0) {
        perror("listen error");
        exit(1);
    }

    while (1) {
        addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
        if (clnt_sock < 0) {
            continue;
        }

        if ((pid = fork()) < 0) {
            close(clnt_sock);
            continue;
        }

        else if (pid > 0) {    // parent process
            puts("create connection");
            close(clnt_sock);
            continue;
        }

        else {                                  // child process
            close(serv_sock);

            // data communication
            while ((str_len = read(clnt_sock, msg, BUFF)) != 0) {
                write(clnt_sock, msg, str_len);
                write(1, msg, str_len);
            }
            puts("release connection");
            close(clnt_sock);
            exit(0);
        }

    }

    return 0;
}

void processHandler(int sig) {  //좀비프로세스 방지
    pid_t pid;
    int state;

    pid = waitpid(-1, &state, WNOHANG);
}