//20220406
//print network ordering address * host name
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char** argv) {

    struct hostent* host;
    struct sockaddr_in addr;
    int i;

    if (argc != 2) {
        printf("Usage: %s [IP]\n", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    printf("%s %x\n", argv[1], addr.sin_addr.s_addr);

    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if (!host) {
        printf("gethostbyaddr() error\n");
        exit(1);
    }

    printf("호스트 이름: %s \n\n", host->h_name);

    return 0;
}