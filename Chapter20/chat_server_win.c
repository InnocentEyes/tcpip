//
// Created by qiuzelin1 on 2022/8/3.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <unistd.h>

void error_handling(char *message);

void send_msg(char *message);

unsigned WINAPI handle(void *arg);


HANDLE mutex;

#define BUF_SIZE 100
#define MAX_CLNT 256
int clnt_cnt = 0;

SOCKET clnt_socks[MAX_CLNT];

/**
 * chat_server_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKADDR_IN serv_addr,clnt_addr;
    SOCKET serv_sock,clnt_sock;
    int clnt_adr_sz;
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("wsa start up error ~");
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == INVALID_SOCKET){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        error_handling("bind error ~");
    }

    if(listen(serv_sock,5) == SOCKET_ERROR){
        error_handling("listen error ~");
    }
    mutex = CreateMutexA(NULL,FALSE,NULL);
    while (1){
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);
        if(clnt_sock == SOCKET_ERROR){
            error_handling("accept error ~");
        }
        WaitForSingleObject(mutex,INFINITE);
        clnt_socks[clnt_cnt++] = clnt_sock;
        ReleaseMutex(mutex);

        _beginthreadex(NULL,0,handle,&clnt_sock,0,NULL);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_addr.sin_addr));
    }
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

unsigned int handle(void *arg){
    SOCKET clnt_sock = *((SOCKET*)arg);
    int str_len;
    int i;
    char message[BUF_SIZE];
    while((str_len = recv(clnt_sock,message,BUF_SIZE,0)) != 0){
        send_msg(message);
    }
    WaitForSingleObject(mutex,INFINITE);
    for(i = 0 ; i < clnt_cnt ; i ++){
        if(clnt_sock == clnt_socks[i]){
            while (i++ < clnt_cnt - 1){
                clnt_socks[i] = clnt_socks[i + 1];
            }
            break;
        }
    }
    clnt_cnt--;
    ReleaseMutex(mutex);
    closesocket(clnt_sock);
    return 0;
}

void send_msg(char *message){
    int i;
    WaitForSingleObject(mutex,INFINITE);
    for(i = 0; i < clnt_cnt ; i++){
        send(clnt_socks[i],message, strlen(message),0);
    }
    ReleaseMutex(mutex);
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}


