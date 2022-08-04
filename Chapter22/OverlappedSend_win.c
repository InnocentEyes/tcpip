//
// Created by qiuzelin1 on 2022/8/4.
//

/**
 * SOCKET WSASocket(int af,int type,int protocol,LPWSAPROTOCOL_INFO lpProtocolInfo.GROUP g,DWORD dwFlags);
 *
 * int WSASend(SOCKET s,LPWSABUF lpBuffers,DWORD dwBufferCount,LPDWORD lpNumberOfBytesSent,DWORD deFlags,LPWSAOVERLAPPED lpOverlapped,LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


void ErrorHandling(char *message);

/**
 * OverlappedSend_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    WSADATA wsadata;
    SOCKET hSokcet;
    SOCKADDR_IN sendAdr;
    WSABUF dataBuf;
    char msg[] = "Network is Computer!";
    int sendBytes = 0;
    WSAEVENT evObj;
    WSAOVERLAPPED overlapped;

    if(argc != 3){
        printf("Usage: %s <IP> <port> \n",argv[0]);
        exit(1);
    }

    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error ~");
    }

    hSokcet = WSASocket(PF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
    memset(&sendAdr,0, sizeof(sendAdr));
    sendAdr.sin_family = AF_INET;
    sendAdr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    sendAdr.sin_port = htons(atoi(argv[2]));

    if(connect(hSokcet, (const struct sockaddr *) &sendAdr, sizeof(sendAdr)) == SOCKET_ERROR){
        ErrorHandling("connect() error!");
    }

    evObj = WSACreateEvent();
    memset(&overlapped,0, sizeof(overlapped));
    overlapped.hEvent = evObj;
    dataBuf.len = strlen(msg) + 1;
    dataBuf.buf = msg;

    if(WSASend(hSokcet, &dataBuf, 1, (LPDWORD) &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
    {
        if(WSAGetLastError() == WSA_IO_PENDING){
            puts("Background data send");
            WSAWaitForMultipleEvents(1,&evObj,TRUE,WSA_INFINITE,FALSE);
            WSAGetOverlappedResult(hSokcet, &overlapped, (LPDWORD) &sendBytes, FALSE, NULL);
        }else{
            ErrorHandling("WSASend() error");
        }
    }

    printf("Send data size: %d \n",sendBytes);
    WSACloseEvent(evObj);
    closesocket(hSokcet);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}