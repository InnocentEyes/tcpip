//
// Created by qiuzelin1 on 2022/6/23.
//

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define  BUF_SIZE 100

void error_handling(char *message);

int main(){

    int read_io;
    int write_io;
    ssize_t read_size;
    ssize_t write_size;
    char message[BUF_SIZE];

    read_io = open("test.dat",O_RDONLY);
    write_io = open("test.dat.bat",O_WRONLY | O_CREAT | O_TRUNC);

    read_size = read(read_io,message, sizeof(message));

    if(read_size == -1){
        error_handling("read() error!");
    }
    printf("read success. read: %s",message);
    write_size = write(write_io,message,sizeof(message) - 1);
    if(write_size == -1){
        error_handling("wirte() error!");
    }
    printf("write success. write: %zd",write_size);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\0',stderr);
    exit(1);
}
