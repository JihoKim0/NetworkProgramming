//20220511
//TCO Non blocking chatting server(test with tcp_chatcli.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 511
#define MAX_SOCK 1024

char* EXIT_STRING = "exit";
char* START_STRING = "Connected to the chat_server\n";
int num_chat = 0;
int clisock_list[MAX_SOCK];
int listen_sock;

void addClient(int s, struct sockaddr_in* newcliaddr);
void removeClient(int s);
int set_nonblock(int sock);
int is_nonblock(int sock);
int tcp_listen(int host, int port, int backlog);
void errquit(char* mesg);

int main(int argc, char* argv[]) {
    char buff[MAXLINE];
    int i, j, nbyte, count;
    int accp_sock;
    int cli_len;
    struct sockaddr_in cli_addr;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
    if (listen_sock < 0) {
        errquit("tcp_listen fail");
    }

    if (set_nonblock(listen_sock) == -1) {  //non-block모드 설정
        errquit("set_nonblock fail");
    }

    for (count = 0; ; count++) {    //클라이언트 대기
        if (count == 100000) {
            putchar('.');
            fflush(stdout);
            count = 0;
        }

        cli_len = sizeof(cli_addr);
        accp_sock = accept(listen_sock, (struct sockaddr*)&cli_addr, &cli_len);
        if (accp_sock == -1 && errno != EWOULDBLOCK) {
            errquit("accept fail");
        }
        else if (accp_sock > 0) {
            if (is_nonblock(accp_sock) != 0 && set_nonblock(accp_sock) < 0) {
                errquit("set_nonblock fail");
            }

            addClient(accp_sock, &cli_addr);
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("User %d added.\n", num_chat);
        }

        for (i = 0; i < num_chat; i++) {
            errno = 0;

            nbyte = recv(clisock_list[i], buff, MAXLINE, 0);
            if (nbyte == 0) {       //수신메세지 없으면 해당 클라이언트 제거
                removeClient(i);
                continue;
            }
            else if (nbyte == -1 && errno == EWOULDBLOCK) {
                continue;
            }

            if (strstr(buff, EXIT_STRING) != NULL) {        //종료메세지 입력시 해당 클라이언트 제거
                removeClient(i);
                continue;
            }

            buff[nbyte] = 0;
            for (j = 0; j < num_chat; j++) {
                send(clisock_list[j], buff, nbyte, 0);  //채팅메세지 전송
            }
            printf("%s\n", buff);
        }
    }

    return 0;
}

void addClient(int sock, struct sockaddr_in* newcliaddr) {      //클라이언트 추가 함수
    char buf[20];
    inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
    printf("new client: %s\n", buf);

    clisock_list[num_chat] = sock;
    num_chat++;
}

void removeClient(int sock) {   //클라이언트 제거 함수
    close(clisock_list[sock]);

    if (sock != num_chat - 1) {
        clisock_list[sock] = clisock_list[num_chat - 1];
    }
    num_chat--;
    printf("Remaining memebers: %d\n", num_chat);
}

int set_nonblock(int sock) {    //소켓을 non-block모드로 설정
    int val;

    val = fcntl(sock, F_GETFL, 0);
    if (fcntl(sock, F_SETFL, val | O_NONBLOCK) < 0) {
        return -1;
    }
    return 0;
}

int is_nonblock(int sock) {     //소켓이 non-block모드인지 판별
    int val;

    val = fcntl(sock, F_GETFL, 0);
    if (val & O_NONBLOCK) {
        return 0;
    }
    return -1;
}

int tcp_listen(int host, int port, int backlog) {
    int sock;
    struct sockaddr_in serv_addr;
    //소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket fail");
        exit(1);
    }
    //소켓 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(host);
    serv_addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind fail");
        exit(1);
    }

    listen(sock, backlog);

    return sock;
}

void errquit(char* mesg) {      //에러처리
    perror(mesg);
    exit(1);
}