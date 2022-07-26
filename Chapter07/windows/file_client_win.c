//
// Created by qiuzelin1 on 2022/7/11.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 30

void error_handling(char *message);

/**
 * windows环境下的半连接 客户端 client
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    SOCKADDR_IN serv_addr;
    SOCKET sock;
    WSADATA wsadata;
    char buf[BUF_SIZE];
    FILE *fp = fopen("receive_win.dat","wb");
    int read_cnt;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("WSAStartup() error!");
    }
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == INADDR_ANY){
        error_handling("socket() error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        error_handling("connect() error!");
    }
    while ((read_cnt = recv(sock,buf,BUF_SIZE,0)) != 0){
        fwrite(buf,1,BUF_SIZE,fp);
    }
    send(sock,"thank you",10,0);
    fclose(fp);
    closesocket(sock);
    WSACleanup();
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
