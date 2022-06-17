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
        FD_ZERO(&read_fds);     //��� ��Ʈ ����
        FD_SET(listen_sock, &read_fds); //�ʱ� ����(Ŭ���̾�Ʈ ������) ����
        for (i = 0; i < num_chat; i++) {        //�������� ��� �ɶ��̾�Ʈ ���� ����
            FD_SET(clisock_list[i], &read_fds);
        }
        maxfd = getmax() + 1;

        puts("wait for client");
        if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0) {   //��ȭ�������н�
            errquit("select fail");
        }

        if (FD_ISSET(listen_sock, &read_fds)) { //����� ���Ͽ��� �Է� �߻�, ä�� ���� ��û ó��
            puts("check point!!");
            accp_sock = accept(listen_sock, (struct sockaddr*)&cliaddr, &addrlen);
            if (accp_sock < 0) {
                errquit("accept fail");
            }

            addClient(accp_sock, &cliaddr); //ä��������ûó��
            send(accp_sock, START_STRING, strlen(START_STRING), 0);
            printf("A user %d added.\n", num_chat);
        }

        for (i = 0; i < num_chat; i++) {        //Ŭ���̾�Ʈ ���Ϲ�ȣ ���ʷ� �˻�
                //��ſ� ���Ͽ��� ä�� �޼��� ����
                //��� �����ڿ��� ä�ø޼��� ���
            if (FD_ISSET(clisock_list[i], &read_fds)) {
                nbyte = recv(clisock_list[i], buff, MAXLINE, 0);
                if (nbyte < 0) {        //���Ÿ޽��� ������ �ش� Ŭ���̾�Ʈ ����
                    removeClient(i);
                    continue;
                }
                buff[nbyte] = 0;

                if (strstr(buff, EXIT_STRING) != NULL) {       //����޼��� ���� �� �ش� Ŭ���̾�Ʈ ����
                    removeClient(i);
                    continue;
                }

                for (j = 0; j < num_chat; j++) {
                    send(clisock_list[j], buff, nbyte, 0); //ä�ø޼��� ���
                }

                printf("%s\n", buff);
            }
        }
    }

    return 0;
}

void addClient(int sock, struct sockaddr_in* newcliaddr) {      //Ŭ���̾�Ʈ �߰� �Լ�
    char buf[20];
    inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
    printf("new client: %s\n", buf);

    clisock_list[num_chat] = sock;
    num_chat++;
}

void removeClient(int sock) {   //Ŭ���̾�Ʈ ���� �Լ�(Ŭ���̾�Ʈ ���� ����Ʈ���� ����)
    close(clisock_list[sock]);

    if (sock != num_chat - 1) {
        clisock_list[sock] = clisock_list[num_chat - 1];
    }
    num_chat--;
    printf("Remaining memebers: %d\n", num_chat);
}

int getmax() {  //�ִ� Ŭ���̾�Ʈ ���� ����Ʈ ũ�� ��ȯ �Լ�
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

    //���� ����
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket fail");
        exit(1);
    }
    //���� ����(�ʱ�ȭ)
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