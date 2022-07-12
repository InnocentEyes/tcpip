//
// Created by qiuzelin1 on 2022/6/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 100

void ErrorHandling(char *message);

int main(int argc,char *argv[]){

    SOCKET clnt_sock;
    SOCKADDR_IN servAddr;
    char message[BUF_SIZE];

    WSADATA wsadata;

    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }

    clnt_sock = socket(PF_INET,SOCK_STREAM,0);
    if(clnt_sock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if (connect(clnt_sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("connect() error!");
    }
    int strLen;
    strLen = recv(clnt_sock,message,sizeof(message) - 1,0);
    if(strLen == 0 || strLen == -1){
        ErrorHandling("recive() message error!");
    }
    printf("recive message : %s",message);
    closesocket(clnt_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\0',stderr);
    exit(1);
}

