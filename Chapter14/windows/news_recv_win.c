//
// Created by qiuzelin1 on 2022/7/26.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define BUF_SIZE 30

void ErrorHandling(char *message);

/**
 * news_recv_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage : %s <groupIp> <port> \n",argv[0]);
        exit(1);
    }
    WSADATA wsadata;
    SOCKADDR_IN recv_addr;
    struct ip_mreq join_addr;
    SOCKET recv_sock;
    char buf[BUF_SIZE];
    int str_len;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error ~");
    }
    recv_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(recv_sock == INVALID_SOCKET){
        ErrorHandling("socket create error ~");
    }
    memset(&recv_addr,0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[2]));

    if(bind(recv_sock, (const struct sockaddr *) &recv_addr, sizeof(recv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind error ~");
    }

    join_addr.imr_interface.S_un.S_addr = htonl(INADDR_ANY);
    join_addr.imr_multiaddr.S_un.S_addr = inet_addr(argv[1]);

    while (1){
        str_len = recvfrom(recv_sock,buf,BUF_SIZE,0,NULL,0);
        if(str_len < 0){
            break;
        }
        buf[str_len] = 0;
        fputs(buf,stdout);
    }
    closesocket(recv_sock);
    WSACleanup();
    return 0;
}

/**
 * 错误处理
 * @param message
 */
void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}