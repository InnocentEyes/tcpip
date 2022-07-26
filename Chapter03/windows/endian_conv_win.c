//
// Created by qiuzelin1 on 2022/6/24.
//

#include <stdio.h>
#include <winsock2.h>

void ErrorHandling(char *message);

int main(){
    WSADATA wsadata;
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!.");
    }
    net_port = htons(host_port);
    net_addr = htonl(host_addr);
    printf("Host ordered port: %#x \n",host_port);
    printf("NetWork ordered port: %#x \n",net_port);
    printf("Host ordered address: %#lx \n",host_addr);
    printf("NetWork ordered address: %#lx \n",net_addr);
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

