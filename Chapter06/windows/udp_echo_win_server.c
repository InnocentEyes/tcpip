//
// Created by qiuzelin1 on 2022/7/8.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void error_handling(char *message);
#define BUF_SIZE 30
/**
 * udp echo server windows版本
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    SOCKADDR_IN serv_adr;
    SOCKADDR_IN clnt_adr;
    SOCKET sock;
    WSADATA wsadata;
    int clnt_adr_sz;
    char message[BUF_SIZE];
    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("WSAStartup() error!");
    }
    sock = socket(PF_INET,SOCK_DGRAM,0);
    if (sock == INVALID_SOCKET){
        error_handling("socket() create error!");
    }
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(sock, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == SOCKET_ERROR){
        error_handling("bind() error!");
    }
    while (1){
        clnt_adr_sz = sizeof(clnt_adr);
        recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *) &clnt_adr, &clnt_adr_sz);
        sendto(sock, message, strlen(message), 0, (const struct sockaddr *) &clnt_adr, clnt_adr_sz);
    }
    closesocket(sock);
    WSACleanup();
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
