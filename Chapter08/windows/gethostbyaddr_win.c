//
// Created by qiuzelin1 on 2022/7/11.
//


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>

void ErrorHandling(char *message);

/**
 * windows环境下的关于gethostbyaddr的使用
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage : %s <ip> \n",argv[0]);
        exit(1);
    }
    int i;
    struct hostent *host;
    WSADATA wsadata;
    struct in_addr addr;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        ErrorHandling("WSAStartup() error!");
    }
    memset(&addr,0, sizeof(addr));
    addr.S_un.S_addr = inet_addr(argv[1]);
    /**
     * 与linux的一样
     */
    host = gethostbyaddr((char *)&addr.S_un.S_addr,4,AF_INET);
    if (!host){
        ErrorHandling("gethost... error");
    }
    printf("Official name: %s \n ",host->h_name);
    for (i = 0;  host->h_aliases[i] ; i++) {
        printf("Aliases %d: %s \n",i + 1,host->h_aliases[i]);
    }
    printf("Address type: %s \n",(host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for (i = 0;  host->h_addr_list[i] ; i++) {
        printf("IP addr %d : %s \n",i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    WSACleanup();
    return 0;
}

void ErrorHandling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
