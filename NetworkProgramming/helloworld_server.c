//20220323 basic server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv) {

    int server_sock;
    int client_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_addr_size;

    char msg[] = "Hello World - Netowrk Programming\n";

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket creation error\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("socket bind error\n");
        exit(1);
    }

    if (listen(server_sock, 5) < 0) {
        perror("socket listen error\n");
        exit(1);
    }

    client_addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_sock < 0) {
        perror("socket accept error\n");
        exit(1);
    }

    write(client_sock, msg, sizeof(msg));
    close(client_sock);

    return 0;
}
