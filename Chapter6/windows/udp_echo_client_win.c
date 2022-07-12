//
// Created by qiuzelin1 on 2022/7/8.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }
    SOCKADDR_IN serv_addr;
    char message[BUF_SIZE];
    int sock;
    int str_len;
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htonl(atoi(argv[2]));
    if (connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("connect() error!");
    }
    while(1){
        fputs("Insert message(q to quit): ",stdout);
        fgets(message,BUF_SIZE,stdin);
        if (!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        send(sock,message, strlen(message),0);
        str_len = recv(sock,message, sizeof(message) - 1,0);
        message[str_len] = '\0';
        printf("Message from server: %s",message);
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
