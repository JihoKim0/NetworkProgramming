//20220406
//print dotted decimal address to 4 byte IP address
//print changed IP address to dotted decimal
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char** argv) {

    char* str;
    char buf[32];
    struct sockaddr_in addr;

    if (argc < 2) {
        printf("Usage: %s [ip address]\n", argv[0]);
        exit(1);
    }

    if (argv[1] > 0) {
        str = argv[1];
    }

    printf("Input dotted decimal IP address: %s\n", str);

    inet_pton(AF_INET, str, &addr.sin_addr);
    printf("inet_pton(%s) = 0x%x\n", str, addr.sin_addr.s_addr);

    inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf));
    printf("inet_ntop(0x%x) = %s\n", addr.sin_addr.s_addr, buf);

    return 0;
}
