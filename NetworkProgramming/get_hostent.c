//20220406
//print host name & host address type & dotted decimal address
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char** argv) {

    int i;
    struct hostent* host;

    if (argc != 2) {
        printf("Usage: %s [addr]\n", argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if (!host) {
        printf("gethost error\n");
        exit(1);
    }

    printf("호스트 이름: %s\n\n", host->h_name);
    printf("호스트 주소타입 번호: %d \n", host->h_addrtype);
    for (i = 0; host->h_addr_list[i]; i++) {
        puts(inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}