//
// Created by qiuzelin1 on 2022/8/3.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>


#define NAME_SIZE 20
#define BUF_SIZE 100

void error_handling(char *message);

/**
 * 处理读
 * @param arg
 * @return
 */
unsigned WINAPI Read(void *arg);

unsigned WINAPI Write(void *arg);


char name[NAME_SIZE] = "[DEFAULT]";

char msg[BUF_SIZE];


/**
 * char_clnt_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET sock;
    SOCKADDR_IN serv_addr;
    HANDLE hThread1,hThread2;
    if(argc != 4){
        printf("Usage : %s <ip> <port> <name> \n");
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("wsa start up error ~");
    }
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == INVALID_SOCKET){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        error_handling("connect error ~");
    }

    sprintf(name,"[%s] ",argv[3]);
    hThread1 = (HANDLE) _beginthreadex(NULL,0,Read,&sock,0,NULL);
    hThread2 = (HANDLE) _beginthreadex(NULL,0,Write,&sock,0,NULL);
    WaitForSingleObject(hThread1,INFINITE);
    WaitForSingleObject(hThread2,INFINITE);
    closesocket(sock);
    WSACleanup();
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

/**
 *
 * @param arg
 * @return
 */
void* Read(void *arg){
    SOCKET clnt_sock = *((SOCKET*)arg);
    char message[NAME_SIZE + BUF_SIZE];
    int strlen;
    while (1){
        strlen = recv(clnt_sock,message,NAME_SIZE + BUF_SIZE,0);
        if(strlen < 1){
            closesocket(clnt_sock);
            exit(0);
        }
        message[strlen] = 0;
        fputs(message,stdout);
    }
    return NULL;
}

/**
 *
 * @param arg
 * @return
 */
void* Write(void *arg){
    SOCKET clnt_sock = *((SOCKET*)arg);
    char message[NAME_SIZE + BUF_SIZE];
    while (1){
        fgets(msg,BUF_SIZE,stdin);
        if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")){
            closesocket(clnt_sock);
            exit(0);
        }
        sprintf(message,"%s %s",name,msg);
        send(clnt_sock,message, strlen(message),0);
    }
    return NULL;
}