//20220406
//print host information & write host information on result.dat
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char** argv) {
    struct hostent* host;
    struct sockaddr_in addr;
    int i;
    int fd = open("result.dat", O_CREAT | O_WRONLY, 00700);
    if (argc != 2) {
        printf("input error\n");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (isdigit(argv[1][0])) {
        host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
        if (!host) {
            printf("input error\n");
            exit(1);
        }
        printf("호스트 이름:%s \n\n", host->h_name);
        write(fd, "호스트 이름:", 15);
        write(fd, host->h_name, strlen(host->h_name));
        write(fd, "\n\n", 2);
    }
    else {
        host = gethostbyname(argv[1]);
        if (!host) {
            printf("input error\n");
            exit(1);
        }
        printf("호스트 이름:%s\n\n", host->h_name);
        write(fd, "호스트 이름:", 15);
        write(fd, host->h_name, strlen(host->h_name));
        printf("호스트 주소타입 번호:%d \n", host->h_addrtype);
        write(fd, "\n\n호스트 주소타입 번호:", 30);;
        write(fd, host->h_addrtype, 2);
        write(fd, "\n", 1);
        for (i = 0; host->h_addr_list[i]; i++) {
            puts(inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
            write(fd, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]), strlen(inet_ntoa(*(struct in_addr*)host->h_addr_list[i])));
            write(fd, "\n", 1);
        }
    }
    close(fd);
    return 0;
}
