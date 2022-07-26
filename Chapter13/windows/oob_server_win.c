//
// Created by qiuzelin1 on 2022/7/26.
//

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#define BUF_SIZE 30
void ErrorHandling(char *message);


/**
 * oob_server_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKADDR_IN acpt_addr;
    SOCKADDR_IN recv_addr;
    SOCKET acpt_sock;
    SOCKET recv_sock;
    int sendAdrSize,strlen;
    int result;
    int fd_max;
    char buf[BUF_SIZE];
    fd_set read,readCpy,except,exceptCpy;
    PTIMEVAL timeout;
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error");
    }
    acpt_sock = socket(PF_INET,SOCK_STREAM,0);
    if(acpt_sock == INVALID_SOCKET){
        ErrorHandling("socket create error");
    }
    memset(&acpt_addr,0, sizeof(acpt_addr));
    acpt_addr.sin_family = AF_INET;
    acpt_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));
    if(bind(acpt_sock, (const struct sockaddr *) &acpt_addr, sizeof(acpt_addr)) == SOCKET_ERROR){
        ErrorHandling("bind error");
    }
    if(listen(acpt_sock,5) == SOCKET_ERROR){
        ErrorHandling("listen error");
    }
    sendAdrSize = sizeof(recv_addr);
    acpt_sock = accept(acpt_sock, (struct sockaddr *) &acpt_addr, &sendAdrSize);
    if(acpt_sock == SOCKET_ERROR){
        ErrorHandling("accpet error~");
    }
    FD_ZERO(&read);
    FD_ZERO(&except);
    FD_SET(acpt_sock,&read);
    fd_max = (int)acpt_sock + 1;
    while (1){
        timeout->tv_sec = 5;
        timeout->tv_usec = 5000;
        readCpy = read;
        exceptCpy = except;
        result = select(fd_max,&readCpy,0,&exceptCpy,timeout);
        if(result == -1){
            ErrorHandling("select error");
            break;
        }
        /**
         * select 函数进行处理
         */
        if(result > 0){
            if(FD_ISSET(recv_sock,&exceptCpy)){
                strlen = recv(recv_sock,buf,BUF_SIZE,MSG_OOB);
                buf[strlen] = 0;
                printf("Urgent message: %s \n",buf);
            }
            if(FD_ISSET(recv_sock,&readCpy)){
                strlen = recv(recv_sock,buf,BUF_SIZE,0);
                if(strlen == 0){
                    closesocket(recv_sock);
                    break;
                }
                buf[strlen] = 0;
                puts(buf);
            }
        }
    }
    closesocket(acpt_sock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

