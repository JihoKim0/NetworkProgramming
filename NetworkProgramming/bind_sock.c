//20220406
//binding socket
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int sock;
    char* ip = "127.0.0.1";
    char* port = "9190";
    struct sockaddr_in addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation error");
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("socket bind error");
        exit(1);
    }
    printf("파일 디스크립터 %d의 소켓에 주소 할당까지 완료!\n\n", sock);
    return 0;
}