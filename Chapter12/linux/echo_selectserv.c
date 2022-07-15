//
// Created by qiuzelin1 on 2022/7/14.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 100


void error_handling(char *message);

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    int serv_sock,clnt_sock;
    int fd_max,fd_num;
    int read_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;
    fd_set reads,cpy_reads;
    struct timeval timeout;
    char message[BUF_SIZE];
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (serv_sock == -1){
        error_handling("socket create error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        printf("bind %d error!\n",serv_addr.sin_port);
        exit(1);
    }
    if(listen(serv_sock,5) == -1){
        error_handling("listen error!");
    }
    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max = serv_sock;
    printf("server start working... \n");
    while (1){
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1,&cpy_reads,0,0,&timeout)) == -1 ){
            break;
        }
        if (fd_num == 0){
            puts("wait...");
            continue;
        }
        puts("resloving....");
        for (int i = 0; i < fd_max + 1; ++i) {
            if (FD_ISSET(i,&cpy_reads)) {
                if (i == serv_sock) {
                    clnt_adr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);
                    if (clnt_sock == -1) {
                        continue;
                    } else {
                        puts("Client Connected...");
                    }
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock) {
                        fd_max = clnt_sock;
                    }
                    printf("connected client: %d \n", clnt_sock);
                }
                    /* 就是读 顺便写! */
                else {
                    read_len = read(i, message, BUF_SIZE);
                    if (read_len == 0) {
                        FD_CLR(i,&reads);
                        close(i);
                        continue;
                    }
                    puts("writing...");
                    write(i,message,read_len); /* echo */
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}



void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
