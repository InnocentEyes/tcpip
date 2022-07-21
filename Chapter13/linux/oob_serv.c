//
// Created by qiuzelin1 on 2022/7/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30

void error_handling(char *message);

void urg_handler(int signo);

/**
 *  MSG_OOB 发送紧急消息 oob_recv.c
 * @param argc
 * @param argv
 * @return
 */

int acpt_sock;
int recv_sock;

int main(int argc,char *argv[]){
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;
    struct sigaction act;
    int str_len;
    char buf[BUF_SIZE];
    acpt_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (acpt_sock == -1){
        error_handling("socket create error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(acpt_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        printf("bind %d error!\n",serv_addr.sin_port);
        exit(1);
    }
    if (listen(acpt_sock,10) == -1){
        error_handling("listen error!");
    }
    clnt_adr_sz = sizeof(clnt_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);


    fcntl(recv_sock,F_SETOWN,getpid());

    /* 向操作系统注册信号处理函数 */
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGURG,&act,0);


    while ((str_len = recv(recv_sock,buf,BUF_SIZE,0)) != 0){
        if (str_len == -1){
            continue;
        }
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void urg_handler(int signo){
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock,buf, sizeof(buf) - 1,MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n",buf);
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

