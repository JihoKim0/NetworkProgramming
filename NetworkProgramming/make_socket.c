//20220330 make tcp, udp socket
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {

    int tcp_sock, udp_sock;

    tcp_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcp_sock < 0) {
        perror("tcp socket error");
        exit(1);
    }

    udp_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp_sock < 0) {
        perror("udp socket error");
        exit(1);
    }

    printf("TCP Socket Descripter: %d\n", tcp_sock);
    printf("UDP Socket Descripter: %d\n", udp_sock);


    close(tcp_sock);
    close(udp_sock);

    return 0;
}