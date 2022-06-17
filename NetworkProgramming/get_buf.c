//20220601
//get socket buffer size
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {

    int sock;
    int snd_buf;
    int rcv_buf;

    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &snd_buf, &len);
    if (state) {
        perror("socket option error");
        exit(1);
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcv_buf, &len);
    if (state) {
        perror("socket option error");
        exit(1);
    }

    printf("RCV SOCKET BUFFER SIZE: %d\n", rcv_buf);
    printf("SND SOCKET BUFFER SIZE: %d\n", snd_buf);

    return 0;
}