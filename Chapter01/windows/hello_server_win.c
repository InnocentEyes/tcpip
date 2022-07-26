//
// Created by qiuzelin1 on 2022/6/23.
//
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

void ErrorHandling(char *message);

int main(int argc,char* argv[]){
    WSADATA wsadata;

    SOCKET hServerSock,hClntSock;
    SOCKADDR_IN servAddr,clntAddr;

    char message[] = "Hello,World!";
    int szClntAddr;

    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    hServerSock = socket(PF_INET,SOCK_STREAM,0);
    if(hServerSock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
//    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if (bind(hServerSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("bind() error!");
    }
    if (listen(hServerSock,5) == SOCKET_ERROR){
        ErrorHandling("listen() error!");
    }
    szClntAddr = sizeof(clntAddr);
    hClntSock = accept(hServerSock, (struct sockaddr *) &clntAddr, &szClntAddr);
    if (hClntSock == INVALID_SOCKET){
        ErrorHandling("accept() error");
    }
    send(hClntSock,message, sizeof(message),0);
    closesocket(hClntSock);
    closesocket(hServerSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\0',stderr);
    exit(1);
}

