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

    if (pipe(fd) < 0) {    //������ ����,  ���� �� ���� ó��
        perror("pipe");
        exit(1);
    }

    if ((pid = fork()) < 0) {      //���μ��� ����, ���� �� ���� ó��
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {   //�ڽ� ���μ���
        close(fd[1]);   //������ ���� �� �ݱ�
        printf("Child : Wait for parent to send data\n");
        if ((n = read(fd[0], buf, MAX_BUF)) < 0) {     //�θ����μ����κ��� �޼��� ����, ���� �� ����ó��
            perror("read");
            exit(1);
        }
        printf("Child : Received data from parent: ");
        fflush(stdout);
        write(STDOUT_FILENO, buf, n);   //������ �޼��� write
    }
    else { //�θ� ���μ���
        close(fd[0]);   //�д� �� �ݱ�
        strcpy(buf, "Hello, World!\n"); //�޼��� ����
        printf("Parent: Send data to child\n");
        write(fd[1], buf, strlen(buf) + 1);       //�޼��� �۽�
    }

    exit(0);
}
