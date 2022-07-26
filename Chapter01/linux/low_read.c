//
// Created by qiuzelin1 on 2022/6/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

void error_handing(char *message);

int main() {
    int fd;
    char buf[BUF_SIZE];
    ssize_t readByte;
    fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        error_handing("open() file error!");
    }
    readByte = read(fd, buf, sizeof(buf));
    if (readByte == -1) {
        error_handing("read() file error!");
    }
    printf("read bytes: %zd, read message: %s",readByte,buf);
    close(fd);
    return 0;
}

void error_handing(char *message) {
    fputs(message, stderr);
    fputc('\0', stderr);
    exit(1);
}

