//
// Created by qiuzelin1 on 2022/6/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 20

void error_handling(char *message);

int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port>",argv[0]);
        exit(1);
    }
    int serv_sock;
    char message[BUF_SIZE];
    struct sockaddr_in address;
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket() error!");
    }
    memset(&address,0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(atoi(argv[2]));
    if(connect(serv_sock, (struct sockaddr *) &address, sizeof(address)) == -1){
        error_handling("connect() error!");
    }
    read(serv_sock,message,sizeof(message) - 1);
    printf("read message from server: %s",message);
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
