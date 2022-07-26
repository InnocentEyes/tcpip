//
// Created by qiuzelin1 on 2022/7/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(char *message);

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int acpt_sock,recv_sock;
    struct sockaddr_in acpt_addr;
    struct sockaddr_in recv_addr;
    socklen_t recv_addr_sz;
    int str_len;
    char buf[BUF_SIZE];
    if (argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    acpt_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(acpt_sock == -1){
        error_handling("socket create error!");
    }
    memset(&acpt_addr,0, sizeof(acpt_addr));
    acpt_addr.sin_family = AF_INET;
    acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));
    if(bind(acpt_sock, (const struct sockaddr *) &acpt_addr, sizeof(acpt_addr)) == -1){
        error_handling("bind error!");
    }
    if(listen(acpt_sock,5) == -1){
        error_handling("listen error!");
    }
    recv_addr_sz = sizeof(recv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr *) &recv_addr, &recv_addr_sz);
    if(recv_sock == -1){
        error_handling("accept error!");
    }
    while (1){
        str_len = recv(recv_sock,buf,BUF_SIZE,MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0){
            break;
        }
    }
    buf[str_len] = 0;
    printf("Read again: %s \n",buf);
    close(acpt_sock);
    close(recv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

