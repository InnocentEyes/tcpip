//
// Created by qiuzelin1 on 2022/6/27.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024
void ErrorHandling(char *message);

int main(int argc,char *argv[]){
    SOCKET serv_sock;
    SOCKET clnt_sock;
    SOCKADDR_IN servAddr;
    SOCKADDR_IN clntAddr;
    int clnt_adr_sz;
    char message[BUF_SIZE];
    WSADATA wsadata;
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("bind() error!");
    }
    if (listen(serv_sock,5) == SOCKET_ERROR){
        ErrorHandling("listen() error!");
    }
    clnt_adr_sz = sizeof(clntAddr);
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clntAddr, &clnt_adr_sz);
        if(clnt_sock == SOCKET_ERROR){
            printf("%d server accept() error.",i);
            exit(1);
        }else{
            printf("%d Client connected.",i);
        }
        while (recv(clnt_sock,message, sizeof(message),0) != 0){
            send(clnt_sock,message, strlen(message),0);
        }
        closesocket(clnt_sock);
    }
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}