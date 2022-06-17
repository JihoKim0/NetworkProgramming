//20220518
//communication using FIFOs(client)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"


void main()
{
    char            fname[MAX_FIFO_NAME];
    int                     fd, sfd, n;
    MsgType         msg;

    sprintf(fname, ".fifo%d", getpid());
    if (mkfifo(fname, 0600) < 0) { //create fifo
        perror("mkfifo");
        exit(1);
    }

    if ((fd = open(fname, O_RDWR)) < 0) {  //open fifo to file descriptor(read&write)
        perror("open");
        exit(1);
    }
    if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) {     //open fifo to socket file descriptor(read&rite)
        perror("open");
        exit(1);
    }

    strcpy(msg.returnFifo, fname);  //modify message
    sprintf(msg.data, "This is a request from %d.", getpid());
    write(sfd, (char*)&msg, sizeof(msg));  //write message to socekt fd
    printf("Sent a request.....");

    if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {   //receive message
        perror("read");
        exit(1);
    }

    printf("Received reply: %s\n", msg.data);

    close(fd);
    close(sfd);

    if (remove(fname) < 0) {       //remove fname
        perror("remove");
        exit(1);
    }
}
