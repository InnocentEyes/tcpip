//
// Created by qiuzelin1 on 2022/7/27.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 30


void error_handling(char *message);

/**
 * 使用标准I/O的echo_server 服务器
 * echo_std_server.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;
    FILE *readFp;
    FILE *writeFp;
    char buf[BUF_SIZE];
    if (argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
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

    for (int i = 0; i < 5; ++i) {
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("accept error ~");
        }
        readFp = fdopen(clnt_sock,"r");
        writeFp = fdopen(clnt_sock,"w");

        while(fgets(buf,BUF_SIZE,readFp)){
            fputs(buf,writeFp);
            fflush(writeFp);
        }
        fclose(readFp);
        fclose(writeFp);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}