//
// Created by qiuzelin1 on 2022/6/24.
//

#include <stdio.h>
#include <arpa/inet.h>

int main(){
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;
    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("host ordered port: %x",host_port);
    printf("net ordered port: %x",net_port);
    printf("host ordered address: %lx",host_addr);
    printf("net ordered address: %lx",net_addr);
    return 0;
}