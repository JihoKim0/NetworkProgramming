//20220518
//IPC between two processes using message queue(client)
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

#define MSG_MODE        (0600)


void main()
{
    int             msqid, n;
    MsgType msg;


    if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0) { //create msg queue
        perror("msgget");
        exit(1);
    }

    msg.type = MSG_REQUEST; //set message type(request)
    sprintf(msg.data, "This is a request from %d.", getpid());
    if (msgsnd(msqid, (char*)&msg, sizeof(msg), 0) < 0) { //send message
        perror("msgsnd");
        exit(1);
    }
    printf("Sent a request.....");

    if ((n = msgrcv(msqid, (char*)&msg, sizeof(msg), MSG_REPLY, 0)) < 0) {        //recieve message
        perror("msgrcv");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);
}
