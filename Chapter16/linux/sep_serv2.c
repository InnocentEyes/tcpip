//
// Created by qiuzelin1 on 2022/7/28.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char *message);

/**
 * sep_serv2.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    FILE *readfp,*writefp;
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE];
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind error ~");
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen error ~");
    }
    clnt_adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);
    if(clnt_sock == -1){
        error_handling("accpet error ~");
    }
    readfp = fdopen(clnt_sock,"r");
    writefp = fdopen(dup(clnt_sock),"w");

    fputs("FROM SERVER: HI~ client? \n",writefp);
    fputs("I love all of the world \n",writefp);
    fputs("You are awesome! \n",writefp);
    fflush(writefp);

    shutdown(fileno(writefp),SHUT_WR);
    fgets(buf, sizeof(buf),readfp);
    fputs(buf,stdout);
    fclose(readfp);
    return 0;
}

/**
 * 错误处理~
 * @param message
 */
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}