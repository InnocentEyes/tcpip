//
// Created by qiuzelin1 on 2022/7/26.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

void ErrorHandling(char *message);
#define TTL 64
#define BUF_SIZE 30

/**
 * new_sender_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKADDR_IN send_addr;
    SOCKET send_sock;
    int timelive = TTL;
    FILE *fp;
    char buf[BUF_SIZE];
    if(argc != 3){
        printf("Usage : %s <groupIp> <port> \n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error ~");
    }
    send_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(send_sock == INVALID_SOCKET){
        ErrorHandling("socket create error ~");
    }
    memset(&send_addr,0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    send_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char *) &timelive, sizeof(timelive));
    if(!(fp = fopen("news.txt","r"))){
        ErrorHandling("fopen error ~");
    }
    while(!feof(fp)){
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock, buf, strlen(buf), 0, (const struct sockaddr *) &send_addr, sizeof(send_addr));
        Sleep(2000);
    }
    closesocket(send_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
