//
// Created by qiuzelin1 on 2022/6/24.
//
#include <stdio.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(){
    char *addr1 = "1.1.1.1";
    char *addr2 = "1.1.1.256";
    WSADATA wsadata;
    unsigned long address;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    address = inet_addr(addr1);
    if(address == INADDR_NONE){
        ErrorHandling("Error occured!");
    }else{
        printf("Network ordered integer addr: %#lx \n",address);
    }
    address = inet_addr(addr2);
    if(address == INADDR_NONE){
        ErrorHandling("Error occured!");
    }else{
        printf("Network ordered integer addr: %#lx \n",address);
    }
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

