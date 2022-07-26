//
// Created by qiuzelin1 on 2022/6/26.
//
// echo_server.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc,char *argv[]){
    int serv_sock;
    int clnt_sock;
    int str_len,i;
    char message[BUF_SIZE];
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    socklen_t  clnt_adr_sz;
    if (argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1){
        error_handling("bind() error");
    }
    if(listen(serv_sock,15) == -1){
        error_handling("listen() error");
    }
    clnt_adr_sz = sizeof(clntAddr);
    for (i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clntAddr,&clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("accept() error");
        }else{
            printf("Connected client %d \n",i + 1);
        }
        while ((str_len = read(clnt_sock,message, sizeof(message))) != 0){
            printf("read : %d \n",str_len);
            write(clnt_sock,message, sizeof(message));
        }
        printf("recive : %d",str_len);
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}