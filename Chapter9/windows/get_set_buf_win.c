//
// Created by qiuzelin1 on 2022/7/12.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void error_handling(char *message);

void ShowSocketBufSize(SOCKET sock);

/**
 * windows 环境下的socket tcp ip的参数替换的影响
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET sock;
    int rcv_buf,snd_buf,state;
    if (WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("WSAStartup() error!");
    }
    sock = socket(PF_INET,SOCK_STREAM,0);
    if (sock == INVALID_SOCKET){
        error_handling("socket error!");
    }
    ShowSocketBufSize(sock);
    rcv_buf = 1024 * 3;
    snd_buf = 1024 * 3;
    state = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&rcv_buf, sizeof(rcv_buf));
    if (state){
        error_handling("setsock.... error");
    }
    state = setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&snd_buf, sizeof(snd_buf));
    if (state){
        error_handling("setsock... error");
    }
    ShowSocketBufSize(sock);
    closesocket(sock);
    WSACleanup();
    return 0;
}

void ShowSocketBufSize(SOCKET sock){
    int rcv_buf,snd_buf,state,len;
    len = sizeof(rcv_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&rcv_buf,&len);
    if (state){
        error_handling("getsocket... error");
    }
    len = sizeof(snd_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&snd_buf,&len);
    if (state){
        error_handling("getsocket... error");
    }
    printf("Input buffer size: %d \n",rcv_buf);
    printf("Output buffer size: %d \n",snd_buf);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}