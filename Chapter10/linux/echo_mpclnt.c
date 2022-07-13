//
// Created by qiuzelin1 on 2022/7/13.
//


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>


void error_handling(char *message);

void write_routine(int sock,char *buf);

void read_routine(int sock,char *buf);

/**
 * echo_mpclnt.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 3){
        printf("Usage : %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    int sock;
    char message[BUFSIZ];
    struct sockaddr_in serv_addr;
    pid_t pid;
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sock == -1){
        error_handling("socket create error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect error!");
    }
    pid = fork();
    if (pid == 0){
        read_routine(sock,message);
    }else{
        write_routine(sock,message);
    }
    close(sock);
    return 0;
}

/**
 * 错误处理
 * @param message
 */
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void write_routine(int sock,char *buf){
    while (1){
        fgets(buf,BUFSIZ,stdin);
        if (!strcmp(buf,"q\n") || !strcmp(buf,"Q\n")){
            shutdown(sock,SHUT_WR);
            break;
        }
        write(sock,buf, strlen(buf));
    }
}

void read_routine(int sock,char *buf){
    while (1){
        int str_len = read(sock,buf,BUFSIZ);
        if (str_len == 0){
            return;
        }
        buf[str_len] = '\0';
        printf("Message from server : %s\n",buf);
    }
}