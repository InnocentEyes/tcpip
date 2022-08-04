//
// Created by qiuzelin1 on 2022/8/4.
//

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>

#define BUF_SIZE 1024

void CALLBACK CompRoutine(DWORD,DWORD,LPWSAOVERLAPPED,DWORD);

void ErrorHandling(char *message);

WSABUF dataBuf;

char buf[BUF_SIZE];

int recvBytes = 0;

/**
 * CmplRoutinesRecv_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET hLisnSock,hRecvSock;
    SOCKADDR_IN lisnAdr,recvAdr;

    WSAOVERLAPPED overlapped;
    WSAEVENT evObj;

    int idx,recvAdrSz,flags = 0;
    if(argc != 2){
        printf("Usage: %s <port> \n",argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error !");
    }

    hLisnSock = WSASocket(PF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
    memset(&lisnAdr,0, sizeof(lisnAdr));
    lisnAdr.sin_family = AF_INET;
    lisnAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    lisnAdr.sin_port = htons(atoi(argv[1]));

    if(bind(hLisnSock, (const struct sockaddr *) &lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }

    if(listen(hLisnSock,5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    recvAdrSz = sizeof(recvAdr);
    hRecvSock = accept(hLisnSock, (struct sockaddr *) &recvAdr, &recvAdrSz);
    if(hRecvSock == INVALID_SOCKET)
        ErrorHandling("accept() error");

    memset(&overlapped,0, sizeof(overlapped));
    dataBuf.len = BUF_SIZE;
    dataBuf.buf = buf;
    evObj = WSACreateEvent();  //Dummy event object

    if(WSARecv(hRecvSock, &dataBuf, 1, (LPDWORD) &recvBytes, (LPDWORD) &flags, &overlapped, CompRoutine) == SOCKET_ERROR){
        if(WSAGetLastError() == WSA_IO_PENDING){
            puts("Background data receive");
        }
    }

    idx = WSAWaitForMultipleEvents(1,&evObj,FALSE,WSA_INFINITE,TRUE);
    if(idx == WAIT_IO_COMPLETION){
        puts("Overlapped I/O Completed");
    } else{
        ErrorHandling("WSARecv() error");
    }

    WSACloseEvent(evObj);
    closesocket(hRecvSock);
    closesocket(hLisnSock);
    WSACleanup();
    return 0;
}

void CALLBACK CompRoutine(DWORD dwError,DWORD szRecvBytes,LPWSAOVERLAPPED lpOverlapped,DWORD flags){
    if(dwError != 0){
        ErrorHandling("CompRoutine error");
    }else{
        recvBytes = szRecvBytes;
        printf("Received message: %s \n",buf);
    }
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}