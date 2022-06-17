//20220601
//get socket type
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc, char* argv[]) {

    int tcp_sock, udp_sock;
    int sock_type = -1;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if (state) {
        perror("sock option error");
        exit(1);
    }
    printf("1st socket type is %d\n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if (state) {
        perror("sock option error");
        exit(1);
    }
    printf("2nd socket type is %d\n", sock_type);

    return 0;
}