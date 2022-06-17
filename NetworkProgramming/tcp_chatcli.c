//20220511
//TCP chatting client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 500
#define NAME_LEN 20

char* EXIT_STRING = "exit";

int tcp_connect(char* serv_ip, unsigned short port);
void errquit(char* mesg);

int main(int argc, char* argv[]) {
    char buff[MAXLINE + NAME_LEN];
    char* buffmesg;
    int maxfd;
    int sock;
    int name_len;
    int nbyte;

    fd_set read_fds;

    if (argc != 4) {
        printf("Usage: %s <serv_ip> <port> <name>\n", argv[0]);
        exit(0);
    }

    sprintf(buff, "[%s]:", argv[3]);
    name_len = strlen(buff);
    buffmesg = buff + name_len;

    sock = tcp_connect(argv[1], atoi(argv[2]));
    if (sock < 0) {
        errquit("tcp_connect fail");
    }

    puts("Connectng to the chat_server...");
    maxfd = sock + 1;

    FD_ZERO(&read_fds);

    while (1) {
        FD_SET(0, &read_fds);   //fdset�� 0�� ����fd ��Ʈ 1�� ����
        FD_SET(sock, &read_fds);        //fdset �� sock�� ���� fd ��Ʈ 0���� ����
        if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0) {   //��ȭ ����
            errquit("select fail");
        }

        if (FD_ISSET(sock, &read_fds)) {        //fdset �� ����fd�� �ش��ϴ� ��Ʈ�� ��Ʈ�Ǿ� �ִ� ���
            if ((nbyte = recv(sock, buffmesg, MAXLINE, 0)) > 0) {  //�޼��� ����
                buffmesg[nbyte] = 0;
                printf("%s\n", buffmesg);
            }
        }

        if (FD_ISSET(0, &read_fds)) {   //����fd�� 0�ΰ��
            if (fgets(buffmesg, MAXLINE, stdin)) {
                if (send(sock, buff, name_len + strlen(buffmesg), 0) < 0) {     //����ó��
                    puts("Error: Write error on socket");
                }

                if (strstr(buffmesg, EXIT_STRING) != NULL) {   //����޼��� ���Ž�
                    puts("Good bye.");
                    close(sock);
                    exit(0);
                }
            }
        }
    }

    return 0;
}

int tcp_connect(char* serv_ip, unsigned short port) {
    struct sockaddr_in serv_addr;
    int sock;
    //���� ����
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket fail");
        exit(1);
    }
    //���� ����
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
    serv_addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {      //���� ����
        perror("connect fail");
        exit(1);
    }

    return sock;
}

void errquit(char* mesg) {      //����ó��
    perror(mesg);
    exit(1);
}