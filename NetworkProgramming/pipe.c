//20220518
//send data from parent to child over a pipe
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

void main()
{
    int             n, fd[2];
    pid_t   pid;
    char    buf[MAX_BUF];

    if (pipe(fd) < 0) {    //파이프 생성,  실패 시 에러 처리
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) < 0) {      //프로세스 생성, 실패 시 에러 처리
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {   //자식 프로세스
        close(fd[1]);   //파이프 쓰는 쪽 닫기
        printf("Child : Wait for parent to send data\n");
        if ((n = read(fd[0], buf, MAX_BUF)) < 0) {     //부모프로세스로부터 메세지 수신, 실패 시 에러처리
            perror("read");
            exit(1);
        }
        printf("Child : Received data from parent: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);   //수신한 메세지 write
    }
    else { //부모 프로세스
        close(fd[0]);   //읽는 쪽 닫기
        strcpy(buf, "Hello, World!\n"); //메세지 수정
        printf("Parent: Send data to child\n");
        write(fd[1], buf, strlen(buf) + 1);       //메세지 송신
    }

    exit(0);
}
