//
// Created by qiuzelin1 on 2022/6/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc,char *argv[]){
    SOCKET serv_sock;
    SOCKADDR_IN servAddr;
    char message[30];
    int idx = 0;
    int read_len = 0;
    int str_len = 0;
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == INVALID_SOCKET){
        ErrorHandling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family =AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if(connect(serv_sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        ErrorHandling("connect() error!");
    }
    while(read_len = recv(serv_sock,&message[idx++], 1,0)){
        if (read_len == -1){
            ErrorHandling("read() error!");
        }
        str_len += read_len;
    }
    printf("Window Message from server: %s \n",message);
    printf("Window Function read call count: %zd \n",str_len);
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
