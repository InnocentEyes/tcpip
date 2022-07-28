//
// Created by qiuzelin1 on 2022/7/27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char *message);

/**
 * 分离io流的作用
 * sep_serv.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    FILE *readfp;
    FILE *writefp;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE] = {0,};
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1){
        error_handling("socket create error ~");
    }
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind error ~");
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen error ~");
    }
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock == -1){
        error_handling("accept error ~");
    }
    readfp = fdopen(clnt_sock,"r");
    writefp = fdopen(clnt_sock,"w");

    fputs("FROM SERVER: Hi~ client? \n",writefp);
    fputs("I love all of the world \n",writefp);
    fputs("You are awesome! \n",writefp);
    fflush(writefp);

    fclose(writefp);
    fgets(buf, sizeof(buf),readfp);
    fputs(buf,stdout);
    fclose(readfp);
    return 0;
}



/**
 *
 * @param message
 */
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}