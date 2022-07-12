//
// Created by qiuzelin1 on 2022/6/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handing(char *message);

int main() {
    int fd;
    char buf[10] = "Let's Go!";
    fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        error_handing("open() error!");
    }
    if(write(fd, buf, sizeof(buf) - 1) == -1){
        error_handing("write() error!");
    }
    close(fd);
    return 0;
}


void error_handing(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

