//20220525
//recieve file from client & save the file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include <signal.h>

#define BUFF_SIZE 4094
#define ALARM_INTERVAL_SEC 1

unsigned int accumulated_size = 0;

void sig_alarm_handler(int signal);

int main(int argc, char* argv[])
{
    //家南 积己
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int server_port;
    int ret;
    char recv_buff[BUFF_SIZE] = { '\0', };

    //颇老器牢磐 积己
    FILE* fp;
    char* file_path = "recv_file";

    pid_t pid;
    int pipe_fd[2];

    /**
     * argv[1] = server_port
     */
    if (argc != 2) {
        printf("Usage : %s [server_port]\n", argv[0]);
        return -1;
    }
    server_port = atoi(argv[1]);

    /* Create a server socket */
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Error) socket()");
        return -1;
    }

    /* Initialize the server socket structure */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Binds the socket to the kernel */
    ret = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0)     //bind 俊矾 贸府
    {
        perror("Error) bind()");
        return -1;
    }
    printf("INFO) Binding Successfull.\n");

    if (listen(server_sock, 5) != 0) {        //wait for client
        perror("ERROR) listen()");
        return -1;
    }
    printf("INFO) Waiting to client.\n");

    /* Accept client connection request */
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_sock == -1) {  //accept 俊矾 贸府
        perror("ERROR) accept()\n");
        return -1;
    }
    else {
        printf("INFO) Accepted to client.\n");
        ret = 0;

        /* Create pipes to communicate with child process*/
        if (pipe(pipe_fd) < 0) {  //pipe 积己
            perror("ERROR) pipe()\n");
            return -1;
        }

        /* Create a child process */
        if ((pid = fork()) < 0) { //fork 俊矾 贸府
            perror("ERROR) fork()\n");
            return -1;
        }
        else if (pid == 0) {
            /**
             * child process
             */
            char child_buff[BUFF_SIZE] = { '\0', };

            close(pipe_fd[1]);

            /* Create a signal_handler that processes I/O every second */
            signal(SIGALRM, sig_alarm_handler);
            alarm(ALARM_INTERVAL_SEC);

            while (1) {
                /* Receive data from parent process */
                ret = read(pipe_fd[0], child_buff, BUFF_SIZE);
                if (ret <= 0) {
                    return -1;
                }

                /* Accumulate the data size received from the parent process */
                accumulated_size += atoi(child_buff);
                memset(child_buff, '\0', BUFF_SIZE);
            }
        }
        else {
            /**
             * parent process
             */
            char parent_buff[BUFF_SIZE] = { '\0', };

            close(pipe_fd[0]);

            /* Open files to save */
            fp = fopen(file_path, "wb");
            if (fp == NULL)
            {
                perror("ERROR) fopen()");
                return -1;
            }

            /* Receive and save the file from the client */
            printf("INFO) Receiving files.\n");
            while (1)
            {
                /* Receive data from client */
                ret = recv(client_sock, recv_buff, BUFF_SIZE, 0);

                /* Send data to child process */
                sprintf(parent_buff, "%d", ret);
                write(pipe_fd[1], parent_buff, BUFF_SIZE);

                /* Write a file */
                fputs(recv_buff, fp);
                memset(recv_buff, '\0', BUFF_SIZE);

                if (ret <= 0)    //end receive
                    break;
            }

            close(client_sock);
            close(server_sock);

            sleep(1);
            kill(pid, SIGKILL);

            printf("INFO) File received successfully.\n");
        }
    }

    return 0;
}

void sig_alarm_handler(int signal)
{
    printf("INFO) Size of data received in 1 sec : %uB/s\n", accumulated_size);
    accumulated_size = 0;

    alarm(ALARM_INTERVAL_SEC);

    return;
}