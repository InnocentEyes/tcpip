//
// Created by qiuzelin1 on 2022/7/1.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);
/**
 * 修改ch04中的客户端程序
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    int clnt_sock;
    struct sockaddr_in servAddr;
    ssize_t str_len;
    char message[BUF_SIZE];
    clnt_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(clnt_sock == -1){
        error_handling("socket() error.");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if(connect(clnt_sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == -1){
        error_handling("connect() error");
    }
    while (1){
        fputs("Input Message or Quit(q): ",stdout);
        fgets(message,BUF_SIZE,stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        str_len = write(clnt_sock,message, strlen(message));
        ssize_t recv_len = 0;
        ssize_t recv_cnt;
        while(str_len > recv_len){
            recv_cnt = read(clnt_sock,&message[recv_len],BUF_SIZE - 1);
            if(recv_cnt == -1){
                error_handling("read() error");
            }
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("recive message from server : %s",message);
    }
    close(clnt_sock);
    return 0;
}

/**
 * 异常输出
 * @param message
 */
void error_handling(char *message){
    /**
     *
     */
    fputs(message,stderr);
    /**
     *
     */
    fputc('\n',stderr);
    /**
     *
     */
    exit(1);
}