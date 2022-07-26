//
// Created by qiuzelin1 on 2022/7/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 30

void ErrorHandling(char *message);

/**
 * oob_clnt_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET hSocket;
    SOCKADDR_IN sendAdr;
    if(argc != 3){
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error~");
    }
    hSocket = socket(PF_INET,SOCK_STREAM,0);
    if(hSocket == INVALID_SOCKET){
        ErrorHandling("socket create error!");
    }
    memset(&sendAdr,0, sizeof(sendAdr));
    sendAdr.sin_family = AF_INET;
    sendAdr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    sendAdr.sin_port = htonl(atoi(argv[2]));
    if(connect(hSocket, (const struct sockaddr *) &sendAdr, sizeof(sendAdr)) == SOCKET_ERROR){
        ErrorHandling("connect error~");
    }
    send(hSocket,"123", strlen("123"),0);
    send(hSocket,"4", strlen("4"),MSG_OOB);
    send(hSocket,"567", strlen("567"),0);
    send(hSocket,"890", strlen("890"),MSG_OOB);
    closesocket(hSocket);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}