//20220518
//communication using FIFOs(server)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"


void SigIntHandler(int signo)   //시그널 핸들러 함수
{
    if (remove(SERV_FIFO) < 0) {
        perror("remove");
        exit(1);
    }

    exit(0);
}


void main()
{
    int fd, cfd, n;
    MsgType msg;

    if (signal(SIGINT, SigIntHandler) == SIG_ERR) {        //시그널 오류 처리
        perror("signal");
        exit(1);
    }

    if (mkfifo(SERV_FIFO, 0600) < 0) {     //create fifo pipe
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(1);
        }
    }

    if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {      //open fifo pipe(read&write)
        perror("open");
        exit(1);
    }

    while (1) {
        if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {   //read message
            if (errno == EINTR) {
                continue;
            }
            else {
                perror("read");
                exit(1);
            }
        }
        printf("Received request: %s.....", msg.data);

        if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) {      //open fifo(write only)
            perror("open");
            exit(1);
        }
        sprintf(msg.data, "This is a reply from %d.", getpid());       //print received message
        write(cfd, (char*)&msg, sizeof(msg));  //write message
        close(cfd);
        printf("Replied.\n");
    }
}