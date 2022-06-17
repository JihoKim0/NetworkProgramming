//20220601
//set socket buffer size
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    int sock;
    int snd_buf = 500;
    int rcv_buf = 1000;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket error");
        exit(1);
    }

    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, sizeof(rcv_buf));
    if (state) {
        perror("set socket option error");
        exit(1);
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, sizeof(snd_buf));
    if (state) {
        perror("set socket option error");
        exit(1);
    }

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
    if (state) {
        perror("get socket option error");
        exit(1);
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &len);
    if (state) {
        perror("get socket option error");
        exit(1);
    }

    printf("RCV SOCKET BUFFER SIZE: %d\n", rcv_buf);
    printf("SND SOCKET BUFFER SIZE: %d\n", snd_buf);

    return 0;
}