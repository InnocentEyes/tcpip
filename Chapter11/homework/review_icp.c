//
// Created by qiuzelin1 on 2022/7/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 100
#define true 1
#define false 0

void read_childProc(int sig);

void error_handling(char *message);

/**
 * icp ch11 task4
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    struct sigaction act;
    char str1[] = "hi,this is child process message. :)";
    char str2[] = "hi,this is parent process message. :)";
    char buf[BUF_SIZE];
    int str_len;
    pid_t pid;
    int fds1[2], fds2[2];
    int isWrite = false;
    act.sa_handler = read_childProc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pipe(fds1);
    pipe(fds2);
    pid = fork();
    if (pid == -1) {
        error_handling("fork process error!");
    }
    if (pid == 0) {
        for (int i = 0; i < 3; ++i) {
            str_len = read(fds1[0], buf, BUF_SIZE);
            buf[str_len] = 0;
            printf("message from parent : %s \n", buf);
            if (!isWrite) {
                write(fds2[1], str1, sizeof(str1));
                isWrite = true;
            }
            write(fds2[1],buf,str_len);
        }
    } else {
        for (int i = 0; i < 3; ++i) {
            if (!isWrite) {
                write(fds1[1], str2, BUF_SIZE);
                isWrite = true;
            }
            write(fds1[1],buf,str_len);
            str_len = read(fds2[0], buf, BUF_SIZE);
            buf[str_len] = 0;
            printf("message from child : %s \n", buf);
        }
        sleep(3);
    }
    return 0;
}

void read_childProc(int sig) {
    int state;
    pid_t pid;
    pid = waitpid(-1, &state, WNOHANG);
    printf("Child Process pided %d Killed!\n", pid);
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
