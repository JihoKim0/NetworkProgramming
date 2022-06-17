//20220525
//send huge file to server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFF_SIZE 4094

int main(int argc, char* argv[])
{
    //家南 积己
    struct sockaddr_in server_addr;
    int sockfd;
    char* server_ip;
    int server_port;
    int ret;
    char send_buff[BUFF_SIZE] = { '\0', };

    FILE* fp;       //颇老器牢磐 积己
    char* file_path;
    int file_tot_size = 0;
    int sent_size = 0;

    /**
     * argv[1] = server_ip
     * argv[2] = server_port
     * argv[3] = file_path_to_transfer
     */
    if (argc != 4) {  //涝仿 坷幅 贸府
        printf("Usage : %s [server_ip] [server_port] [file_path_to_trasnfer]\n", argv[0]);
        return -1;
    }
    //涝仿 贸府
    server_ip = argv[1];
    server_port = atoi(argv[2]);
    file_path = argv[3];

    /* Open files to transfer */
    fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        perror("ERROR) fopen()");
        return -1;
    }

    /* Get file total size */
    fseek(fp, 0, SEEK_END);
    file_tot_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* Check file total size */
    if (file_tot_size < 50000000)
    {
        printf("ERROR) The file size is less then 50M.\n");
        return -1;
    }

    /* Create a socket to connect to the server */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("ERROR) socket()");
        return -1;
    }

    /* Initialize the server socket structure */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    /* Connecting to a server using a socket */
    ret = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1)
    {
        perror("ERROR) connect()");
        return -1;
    }
    printf("INFO) Connected to server.\n");

    /* File sending */
    printf("INFO) Sending files.\n");
    while (file_tot_size > sent_size) {
        ret = fread(send_buff, 1, BUFF_SIZE, fp);
        sent_size += ret;
        send(sockfd, send_buff, ret, 0);

        memset(send_buff, '\0', BUFF_SIZE);
    }
    printf("INFO) File send successfully. \n");

    close(sockfd);
    printf("INFO) Disconnected from the server. \n");

    return 0;
}