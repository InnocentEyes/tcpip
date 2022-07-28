//
// Created by qiuzelin1 on 2022/7/27.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);
/**
 * sep_clnt.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    FILE *readfp;
    FILE *writefp;
    char buf[BUF_SIZE] = {0,};
    if(argc != 3){
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock == -1){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect error ~");
    }
    readfp = fdopen(sock,"r");
    writefp = fdopen(sock,"w");

    while (fgets(buf,BUF_SIZE,readfp)){
        fputs(buf,stdout);
        fflush(stdout);
    }
    fputs("From CLIENT: Thank you ~",writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}