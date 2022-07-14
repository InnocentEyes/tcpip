//
// Created by qiuzelin1 on 2022/7/13.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


void error_handling(char *message);


/**
 * 检验fork之后复制的文件描述符整数值是否与原文件描述符整数值相同
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    int serv_sock;
    pid_t pid;
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (serv_sock == -1){
        error_handling("socket create error!");
    }
    pid = fork();
    if (pid == 0){
        printf("Hi,I'm Child process. my server socket is %d \n",pid);
    }
    else{
        printf("I'm father process. my server socket is %d \n",pid);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}