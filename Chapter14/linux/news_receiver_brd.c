//
// Created by qiuzelin1 on 2022/7/26.
//


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUF_SIZE 30
void error_handling(char *message);
/**
 * news_receiver_brd.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int recv_sock;
    struct sockaddr_in recv_adr;
    ssize_t  str_len;
    char buf[BUF_SIZE];
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    recv_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(recv_sock == -1){
        error_handling("socket create error ~");
    }
    memset(&recv_adr,0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(recv_sock, (const struct sockaddr *) &recv_adr, sizeof(recv_adr)) == -1){
        error_handling("bind error ~");
    }
    while (1){
        str_len = recvfrom(recv_sock,buf,BUF_SIZE,0,NULL,0);
        if(str_len < 0){
            break;
        }
        buf[str_len] = 0;
        fputs(buf,stdout);
    }
    close(recv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}