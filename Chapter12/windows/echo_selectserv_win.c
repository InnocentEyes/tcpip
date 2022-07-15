//
// Created by qiuzelin1 on 2022/7/15.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 100

void ErrorHandling(char *message);

/**
 * windows环境下的单进程I/O复用的实现
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    SOCKET serv_sock;
    SOCKET clnt_sock;
    SOCKADDR_IN serv_addr;
    SOCKADDR_IN clnt_addr;
    int clnt_addr_sz;
    struct timeval timeout;
    fd_set cpy_reads,reads;
    WSADATA wsadata;
    int fd_max,fd_num;
    char message[BUF_SIZE];
    int str_len;
    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup create error!");
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if (serv_sock == INVALID_SOCKET){
        ErrorHandling("socket error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        ErrorHandling("bind error!");
    }
    if (listen(serv_sock,15) == SOCKET_ERROR){
        ErrorHandling("listen error!");
    }
    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max = serv_sock;

    while(1){
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        if ((fd_num = select(fd_max + 1,&cpy_reads,0,0,&timeout)) == -1){
            break;
        }
        if (fd_num == 0){
            continue;
        }

        for (int i = 0; i < fd_max + 1; ++i) {
            if (FD_ISSET(i,&cpy_reads)){
                if (i == serv_sock){
                    clnt_addr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_sz);
                    if (clnt_sock == SOCKET_ERROR){
                        continue;
                    }
                    FD_ISSET(clnt_sock,&reads);
                    if (fd_max < clnt_sock){
                        fd_max = clnt_sock;
                    }
                    puts("connected client....");
                }
                else
                {
                    str_len = recv(i,message,BUF_SIZE,0);
                    if (str_len == 0){
                        FD_CLR(i,&reads);
                        closesocket(i);
                        continue;
                    }
                    send(i,message,str_len,0); /* 回声 echo */
                }
            }
        }

    }
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}