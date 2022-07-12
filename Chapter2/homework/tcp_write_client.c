//
// Created by qiuzelin1 on 2022/6/24.
//

//
// Created by qiuzelin1 on 2022/6/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(1);
    }
    int serv_sock;
    ssize_t read_len;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket() error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error!");
    }
    for (int i = 0; i < 3000; ++i) {
        printf("wait read count: %d \n",i);
    }
    read_len = read(serv_sock,message, sizeof(message));
    if(read_len == -1){
        error_handling("read() error!");
    }
    printf("Message from server: %s \n",message);
    printf("Function read call count: %zd \n",read_len);
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

