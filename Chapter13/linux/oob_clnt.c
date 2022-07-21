//
// Created by qiuzelin1 on 2022/7/15.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUF_SIZE 30

void error_handling(char *message);

/**
 * MSG_OOB 发送紧急的消息 oob_clnt.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 3){
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    int sock;
    struct sockaddr_in recv_adr;
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sock == -1){
        error_handling("socket create error!");
    }
    memset(&recv_adr,0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_adr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (const struct sockaddr *) &recv_adr, sizeof(recv_adr)) == -1){
        error_handling("connect error!");
    }
    write(sock,"123", strlen("123"));
    send(sock,"4", strlen("4"),MSG_OOB);
    write(sock,"567", strlen("567"));
    send(sock,"890", strlen("890"),MSG_OOB);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}