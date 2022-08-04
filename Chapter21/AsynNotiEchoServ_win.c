//
// Created by qiuzelin1 on 2022/8/3
//

/**
 * WSAEVENT WSACreateEvent(void);
 *
 * BOOL WSACloseEvent(WSAEVENT hEvent);
 *
 * DWORD WSAWaitForMultipleEvents(DWORD cEvents,const WSAEVENT* lphEvents,BOOL fWaitALL,DWORD dwTimeout,BOOL fAlertable);
 *
 * int WSAEnumNetworkEvents(SOCKET s,WSAEVENT hEventObject,LPWSANETWORKEVENTS lpNetworkEvents);
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 100
#define WSAEVENT HANDLE


void ErrorHandling(char *message);
void CompressSockets(SOCKET hSockArr[],int idx,int total);
void CompressEvents(WSAEVENT hEventArr[],int idx,int total);

/**
 * AsynNotiEchoServ_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]) {
    WSADATA wsadata;
    SOCKET hServSock,hClntSock;
    SOCKADDR_IN servAdr,clntAdr;

    SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
    WSAEVENT newEvent;
    WSANETWORKEVENTS netEvents;

    int numOfClntSock = 0;
    int strlen,i;
    int posInfo,startIdx;
    int clntAdrLen;
    char msg[BUF_SIZE];

    if(argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("wsa start up error ~");
    }

    hServSock = socket(PF_INET,SOCK_STREAM,0);
    memset(&servAdr,0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if(bind(hServSock, (const struct sockaddr *) &servAdr, sizeof(servAdr)) == SOCKET_ERROR){
        ErrorHandling("bind() error");
    }
    if(listen(hServSock,5) == SOCKET_ERROR){
        ErrorHandling("listen() error");
    }

    newEvent = WSACreateEvent();
    if(WSAEventSelect(hServSock,newEvent,FD_ACCEPT) == SOCKET_ERROR){
        ErrorHandling("WSAEventSelect() error");
    }

    hSockArr[numOfClntSock] = hServSock;
    hEventArr[numOfClntSock] = newEvent;

    numOfClntSock++;

    while (1){
        posInfo = WSAWaitForMultipleEvents(numOfClntSock,hEventArr,FALSE,WSA_INFINITE,FALSE);
        startIdx = posInfo = WSA_WAIT_EVENT_0;

        for (i = startIdx; i < numOfClntSock; i++) {
            int sigEventIdx = WSAWaitForMultipleEvents(1,&hEventArr[i],TRUE,0,FALSE);
            if((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT)){
                continue;
            }
            else
            {
                sigEventIdx = i;
                WSAEnumNetworkEvents(hSockArr[sigEventIdx],hEventArr[sigEventIdx],&netEvents);
                if(netEvents.lNetworkEvents & FD_ACCEPT){ //请求连接时
                    if(netEvents.iErrorCode[FD_ACCEPT_BIT] != 0){
                        puts("Accept Error");
                        break;
                    }
                    clntAdrLen = sizeof(clntAdr);
                    hClntSock = accept(hSockArr[sigEventIdx], (struct sockaddr *) &clntAdr, &clntAdrLen);
                    newEvent = WSACreateEvent();
                    WSAEventSelect(hClntSock,newEvent,FD_READ | FD_CLOSE);

                    hEventArr[numOfClntSock] = newEvent;
                    hSockArr[numOfClntSock] = hClntSock;
                    numOfClntSock++;
                    puts("connected new client...");
                }

                if(netEvents.lNetworkEvents & FD_READ){ //接受数据时
                    if(netEvents.iErrorCode[FD_READ_BIT] != 0){
                        puts("Read Error");
                        break;
                    }
                    strlen = recv(hSockArr[sigEventIdx],msg, sizeof(msg),0);
                    send(hSockArr[sigEventIdx],msg,strlen,0);
                }

                if(netEvents.lNetworkEvents & FD_CLOSE) //断开连接时
                {
                    if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
                    {
                        puts("Close Error");
                        break;
                    }
                    WSACloseEvent(hEventArr[sigEventIdx]);
                    closesocket(hSockArr[sigEventIdx]);

                    numOfClntSock--;
                    CompressSockets(hSockArr,sigEventIdx,numOfClntSock);
                    CompressEvents(hEventArr,sigEventIdx,numOfClntSock);
                }
            }
        }
    }
    WSACleanup();
    return 0;
}//结束此函数

void CompressSockets(SOCKET hSockArr[],int idx,int total){
    int i;
    for(i = idx;i < total; i++)
        hSockArr[i] = hSockArr[i + 1];
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}