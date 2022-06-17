//20220518
//IPC between two processes using message queue(server)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

#define MSG_MODE        (0600 | IPC_CREAT)


void main()
{
    int             msqid, n;
    MsgType msg;


    if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0) { //create message queue
        perror("msgget");
        exit(1);
    }

    if ((n = msgrcv(msqid, (char*)&msg, sizeof(msg), 0, 0)) < 0) {       //receive data from msg queue
        perror("msgrcv");
        exit(1);
    }
    printf("Received request: %s.....", msg.data);

    msg.type = MSG_REPLY;
    sprintf(msg.data, "This is a reply from %d.", getpid());
    if (msgsnd(msqid, (char*)&msg, sizeof(msg), 0) < 0) { //send message
        perror("msgsnd");
        exit(1);
    }
    printf("Replied.\n");

    sleep(1);       //sleep 1 seconds

    if (msgctl(msqid, IPC_RMID, 0) < 0) {  //remove message queue
        perror("msgctl");
        exit(1);
    }
}
