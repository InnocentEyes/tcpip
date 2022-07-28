//
// Created by qiuzelin1 on 2022/7/27.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char *message);

/**
 * dup dup2 //同进程内复制文件描述符
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    int cfd1, cfd2;
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    cfd1 = dup(1);
    cfd2 = dup2(cfd1,7);
    printf("fd1=%d,fd2=%d \n", cfd1, cfd2);
    write(cfd1,str1, sizeof(str1));
    write(cfd2,str2, sizeof(str2));

    close(cfd1);
    close(cfd2);
    write(1,str1, sizeof(str1));
    close(1);
    write(1,str1, sizeof(str2));
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}