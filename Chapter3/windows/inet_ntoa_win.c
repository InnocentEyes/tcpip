//
// Created by qiuzelin1 on 2022/6/24.
//
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(int argc,char *argv[]){
    SOCKADDR_IN sock;
    unsigned long address = htonl(0x12345678);
    sock.sin_addr.S_un.S_addr = address;
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    char *p = inet_ntoa(sock.sin_addr);
    char strArr[20];
    strcpy(strArr,p);
    printf("Dotted-Decimal notation1 %s \n",strArr);
    printf("Dotted-Decimal notation1 %s \n",p);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
