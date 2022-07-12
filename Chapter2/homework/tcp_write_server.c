//
// Created by qiuzelin1 on 2022/6/24.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc,char *argv[]){
    if(argc != 2){
        printf("Usage: %s <port> \n",argv[0]);
        exit(1);
    }
    int serv_sock;
    int clnt_sock;
    ssize_t write_len = 0;
    ssize_t str_len = 0;
    int idx = 0;
    char message[] = "Hello World!";
    socklen_t szClntAddr;
    struct sockaddr_in servAddr,clntAddr;
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == -1){
        error_handling("bind() error!");
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen() error!");
    }
    szClntAddr = sizeof(clntAddr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clntAddr, &szClntAddr);
    if(clnt_sock == -1){
        error_handling("accept() error!");
    }
    while (write_len = write(clnt_sock,&message[idx++],1)){
        if(write_len == -1){
            error_handling("write() error!");
        }
        str_len += write_len;
    }
    printf("Function write call count: %zd \n",str_len);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}