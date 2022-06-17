//20220406
//print TCP Telnet port number to network ordering & host ordering
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    struct servent* servent;
    servent = getservbyname("telnet", "tcp");
    if (servent == 0) {
        perror("getservbyname() error");
        exit(1);
    }

    printf("TCP Telnet Port Number (��Ʈ��ũ ����): %d\n", servent->s_port);
    printf("TCP Telnet Port Number (ȣ��Ʈ ����): %d\n", ntohs(servent->s_port));

    return 0;
}