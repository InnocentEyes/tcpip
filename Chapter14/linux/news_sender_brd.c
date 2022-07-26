//
// Created by qiuzelin1 on 2022/7/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30


void error_handling(char *message);

/**
 * news_sender_brd.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage : %s <groupIp> <port> \n",argv[0]);
        exit(1);
    }
    int send_sock;
    struct sockaddr_in broad_addr;
    int so_brd = 1;
    FILE *fp;
    char buf[BUF_SIZE];
    send_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(send_sock == -1){
        error_handling("socket create error ~");
    }

    memset(&broad_addr,0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,&so_brd, sizeof(so_brd));
    if(!(fp = fopen("./news.txt","r"))){
        error_handling("fopen error~");
    }
    while (!feof(fp)){
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock,buf, strlen(buf),0,(const struct sockaddr *)&broad_addr, sizeof(broad_addr));
        sleep(2);
    }
    close(send_sock);
    return 0;
}

/**
 * 错误处理
 * @param message
 */
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}