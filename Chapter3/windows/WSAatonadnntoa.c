//
// Created by qiuzelin1 on 2022/6/24.
//
#undef UNICODE
#undef _UNIICODE
#include <stdio.h>
#include <winsock2.h>
void ErrorHandling(char *message);

int main(){
    char *strAddr = "203.211.218.102.9190";
    char strAddrBuf[50];
    SOCKADDR_IN servAddr;
    int size;
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    size = sizeof(servAddr);
    WSAStringToAddressA(strAddr, AF_INET, NULL, (LPSOCKADDR) &servAddr, &size);

    size = sizeof(strAddrBuf);
    WSAAddressToStringA((LPSOCKADDR) &servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
