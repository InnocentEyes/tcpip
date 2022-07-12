//
// Created by qiuzelin1 on 2022/6/25.
//
//ch04_server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);


int main(int argc,char *argv[]){
    int serv_sock;
    int clnt_sock;
    socklen_t clntAddrSz;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    char *message = "Hello,World!";
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket() error!.");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1){
        error_handling("bind() error!.");
    }
    if(listen(serv_sock,15) == -1){
        error_handling("listen() error!");
    }
    clntAddrSz = sizeof(clntAddr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clntAddr, &clntAddrSz);
    if(clnt_sock == -1){
        error_handling("accept() error!");
    }
    write(clnt_sock,message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}