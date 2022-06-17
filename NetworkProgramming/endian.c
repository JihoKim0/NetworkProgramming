//20220330
//print:
//host orderd port -> network orderd port
//host orderd address -> network orderd address
#include <stdio.h>
#include <netinet/in.h>

int main(int argc, char** argv) {
    short host_port = 0x1234;
    short net_port;

    long host_address = 0x12345678;
    long net_address;

    net_port = htons(host_port);
    net_address = htonl(host_address);

    printf("Host ordered port: %x \n", host_port);
    printf("=> Network ordered port: %x \n\n", net_port);

    printf("Host ordered address: %lx \n", host_address);
    printf("=> Network ordered address: %lx \n\n", net_address);

    return 0;
}