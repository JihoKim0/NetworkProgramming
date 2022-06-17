//20220330
//change basic address to network ordering address
//change network ordering to host ordering
//make exception process for input error
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

void check_address(char* p)
{
    int count = 0;
    char* ptr = strtok(p, ".");
    while (ptr != NULL) {
        if (atoi(ptr) > 255) {
            printf("input error\n");
            exit(1);
        }
        count++;
        ptr = strtok(NULL, ".");
    }
    if (count != 4) {
        printf("input error\n");
        exit(1);
    }
}


int main(int argc, char** argv) {
    char* addr = argv[1];
    struct sockaddr_in sock_addr;
    int rv;
    char* caddr;
    char p[20];
    strcpy(p, addr);

    check_address(p);

    rv = inet_aton(addr, &sock_addr.sin_addr);
    if (rv < 0) {
        perror("network ordering error");
        exit(1);
    }
    printf("Address: %s => Net ordering: %x \n\n", addr, sock_addr.sin_addr.s_addr);

    sock_addr.sin_addr.s_addr = inet_addr(addr);
    caddr = inet_ntoa(sock_addr.sin_addr);
    if (caddr == 0) {
        perror("host ordering error");
        exit(1);
    }
    printf("Network: %x => Host ordering: %s \n\n", sock_addr.sin_addr.s_addr, caddr);

    return 0;
}
