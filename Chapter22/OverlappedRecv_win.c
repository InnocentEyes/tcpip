//
// Created by qiuzelin1 on 2022/8/4.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char *message);

/**
 * OverlappedRecv_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET hLisnSock,hRecvSock;
    SOCKADDR_IN lisnAdr,recvAdr;
    int recvAdrSz;

    WSABUF dataBuf;
    WSAEVENT evObj;
    WSAOVERLAPPED overlapped;

    char buf[BUF_SIZE];
    int recvBytes = 0,flags = 0;
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error ~");
    }
    hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(hLisnSock == INVALID_SOCKET){
        ErrorHandling("wsa socket create error ~");
    }
    memset(&lisnAdr,0, sizeof(lisnAdr));
    lisnAdr.sin_family = AF_INET;
    lisnAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    lisnAdr.sin_port = htons(atoi(argv[1]));

    if(bind(hLisnSock, (const struct sockaddr *) &lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR){
        printf("bind 127.0.0.1 %d error !",lisnAdr.sin_port);
        exit(1);
    }

    if(listen(hLisnSock,5) == SOCKET_ERROR){
        ErrorHandling("listen error ~");
        exit(1);
    }

    recvAdrSz = sizeof(recvAdr);
    hRecvSock = accept(hLisnSock, (struct sockaddr *) &recvAdr, &recvAdrSz);
    if(hRecvSock == SOCKET_ERROR){
        ErrorHandling("accept error ~");
    }
    memset(&overlapped,0, sizeof(overlapped));
    evObj = WSACreateEvent();
    overlapped.hEvent = evObj;
    dataBuf.buf = buf;
    dataBuf.len = BUF_SIZE;

    if(WSARecv(hRecvSock, &dataBuf, 1, (LPDWORD) &recvBytes, (LPDWORD) &flags, &overlapped, NULL) == SOCKET_ERROR){
        if(WSAGetLastError() == WSA_IO_PENDING){
            puts("background data recive");
            WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
            /**
             * WSAGetOverlappedResult 最后一个参数 lpdwFlags
             * 调用WSARecv函数时,用于获取附加信息(例如OOB信息)。如果不需要，可以传递NULL
             */
            WSAGetOverlappedResult(hRecvSock, &overlapped, (LPDWORD) &recvBytes, FALSE, NULL);
        }
        else{
            puts("WSARecv() error ~");
        }
    }

    printf("received data : %s",buf);
    WSACloseEvent(evObj);
    closesocket(hRecvSock);
    closesocket(hLisnSock);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}