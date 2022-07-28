//
// Created by qiuzelin1 on 2022/7/27.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);

/**
 * 使用标准I/O重现的echo_std_clnt.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    /**
     * 变量定义
     */
    FILE *readFp;
    FILE *writeFp;
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];

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

    /**
     * 连接
     */
    if(connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect error ~");
    }

    readFp = fdopen(sock,"r");
    writeFp = fdopen(sock,"w");

    while (1){
        fputs("Input message(Q to quit): ",stdout);
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n")){
            break;
        }
        fputs(buf,writeFp);
        fflush(writeFp);
        fgets(buf,BUF_SIZE,readFp);
        printf("Message from server: %s",buf);
    }
    fclose(readFp);
    fclose(writeFp);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

