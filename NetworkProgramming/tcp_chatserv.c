//20220511
//TCP chatting server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 511
#define MAX_SOCK 1024

char* EXIT_STRING = "exit";
char* START_STRING = "Connected to the chat_server\n";
int maxfd;
int num_chat = 0;
int clisock_list[MAX_SOCK];
int listen_sock;

void addClient(int sock, struct sockaddr_in* newcliaddr);
int getmax();
void removeClient(int sock);
int tcp_listen(int host, int port, int backlog);
void errquit(char* mesg);

int main(int argc, char* argv[]) {
    struct sockaddr_in cliaddr;
    char buff[MAXLINE + 1];
    int i, j, nbyte;
    int accp_sock;
    int addrlen = sizeof(struct sockaddr_in);
    fd_set read_fds;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);

    while (1) {
        FD_ZERO(&read_fds);     //모든 비트 지움
        FD_SET(listen_sock, &read_fds); //초기 소켓(클라이언트 참가용) 선택
        for (i = 0; i < num_chat; i++) {        //참가중인 모든 믈라이언트 소켓 선택
            FD_SET(clisock_list[i], &read_fds);
        }
        maxfd = getmax() + 1;

        puts("wait for client");
        if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0) {   //변화감지실패시
            errquit("select fail");
        }

        if (FD_ISSET(listen_sock, &read_fds)) { //연결용 소켓에서 입력 발생, 채팅 참가 신청 처리
            puts("check point!!");
            accp_sock = accept(listen_sock, (struct sockaddr*)&cliaddr, &addrlen);
            if (accp_sock < 0) {
                errquit("accept fail");
            }

            addClient(accp_sock, &cliaddr); //채팅참가신청처리
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("A user %d added.\n", num_chat);
        }

        for (i = 0; i < num_chat; i++) {        //클라이언트 소켓번호 차례로 검색
                //통신용 소켓에서 채팅 메세지 수신
                //모든 참가자에게 채팅메세지 방송
            if (FD_ISSET(clisock_list[i], &read_fds)) {
                nbyte = recv(clisock_list[i], buff, MAXLINE, 0);
                if (nbyte < 0) {        //수신메시지 오류시 해당 클라이언트 제거
                    removeClient(i);
                    continue;
                }
                buff[nbyte] = 0;

                if (strstr(buff, EXIT_STRING) != NULL) {       //종료메세지 수신 시 해당 클라이언트 제거
                    removeClient(i);
                    continue;
                }

                for (j = 0; j < num_chat; j++) {
                    send(clisock_list[j], buff, nbyte, 0); //채팅메세지 방송
                }

                printf("%s\n", buff);
            }
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

void removeClient(int sock) {   //클라이언트 제거 함수(클라이언트 소켓 리스트에서 제거)
    close(clisock_list[sock]);

    if (sock != num_chat - 1) {
        clisock_list[sock] = clisock_list[num_chat - 1];
    }
    num_chat--;
    printf("Remaining memebers: %d\n", num_chat);
}

int getmax() {  //최대 클라이언트 소켓 리스트 크기 반환 함수
    int max = listen_sock;
    int i;

    for (i = 0; i < num_chat; i++) {
        if (clisock_list[i] > max) {
            max = clisock_list[i];
        }
    }
    return max;
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
    //소켓 설정(초기화)
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

void errquit(char* mesg) {
    perror(mesg);
    exit(1);
}