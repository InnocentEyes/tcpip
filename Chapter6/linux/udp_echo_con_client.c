//
// Created by qiuzelin1 on 2022/7/8.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);

#define BUF_SIZE 30
/**
 * udp 已连接套接字 upd_echo_con_client
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 3){
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(1);
    }
    int sock;
    struct sockaddr_in serv_adr;
    char message[BUF_SIZE];
    ssize_t str_len;
    sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock == -1){
        error_handling("socket() error!");
    }
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error!");
    }
    while (1){
        fputs("Insert message(q to quit)",stdout);
        fgets(message,BUF_SIZE,stdin);
        if (!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        write(sock,message, strlen(message));
        str_len = read(sock,message,BUF_SIZE);
        message[str_len] = '\0';
        printf("Message recv is : %s",message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
