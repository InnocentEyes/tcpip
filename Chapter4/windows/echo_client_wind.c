//
// Created by qiuzelin1 on 2022/6/27.
//
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#define BUF_SIZE 1024

void ErrorHandling(char *message);

int main(int argc,char *argv[]){
    SOCKADDR_IN servAddr;
    SOCKET sock;
    int str_len;
    WSADATA wsadata;
    char message[BUF_SIZE];
    if(argc != 3){
        printf("Usage : %s <IP> <Port> \n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error.");
    }
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("connect() error!");
    }
    while (1){
        fputs("Input Message(Q to quit): ",stdout);
        fgets(message, sizeof(message),stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        send(sock,message, strlen(message),0);
        str_len = recv(sock,message, sizeof(message),0);
        message[str_len] = '\0';
        printf("recive message from : %s",message);
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
