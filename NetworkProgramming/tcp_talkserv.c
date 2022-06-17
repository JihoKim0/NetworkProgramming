//20220504
//connect with client, then call fork()
//send input to client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF 100

int main(int argc, char** argv) {
    struct sockaddr_in serv_addr, clnt_addr;
    int serv_sock;
    int clnt_sock;
    int addr_size;
    int n;
    pid_t pid;
    char msg[BUFF];


    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    //家南 积己
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        perror("socket() error");
        exit(1);
    }
    //家南 汲沥(檬扁拳)
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind() error");
        exit(1);
    }
    //client 夸没 措扁
    puts("Server waits a client...");
    if (listen(serv_sock, 1) < 0) {
        perror("listen() error");
        exit(1);
    }

    //client 夸没 贸府
    addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
    if (clnt_sock < 0) {
        perror("accept() error");
        exit(1);
    }
    puts("A client connects to the server...");

    if ((pid = fork()) < 0) {
        perror("fork() error");
        exit(1);
    }

    else if (pid > 0) {     // parent process
        while (fgets(msg, sizeof(msg), stdin) != 0) {
            if (strstr(msg, "exit") != 0) {
                puts("Good bye!");
                close(clnt_sock);
                exit(0);
            }
        }
    }

    else {                          // child process
        while (1) {
            if ((n = read(clnt_sock, msg, BUFF)) < 0) {
                perror("read() error");
                close(clnt_sock);
                exit(1);
            }
            msg[n] = 0;

            if (strstr(msg, "exit") != 0) {
                break;
            }

            printf("%s", msg);
        }

    }

    close(serv_sock);
    close(clnt_sock);

    return 0;
}