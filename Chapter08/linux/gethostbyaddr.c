//
// Created by qiuzelin1 on 2022/7/11.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

/**
 * 利用ip地址获取包括域名在内的相关信息
 * @param agrc
 * @param agrv
 * @return
 */
int main(int agrc,char *agrv[]){
    if (agrc != 2){
        printf("Usage : %s <IP>\n",agrv[0]);
        exit(1);
    }
    int i;
    struct hostent *host;
    struct sockaddr_in addr;
    memset(&addr,0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(agrv[1]);
    /**
     * 第一个参数: 含有IP地址信息的in_addr结构体指针。为了同时传递IPv4地址以外的其他信息,该变量的类型声明为char指针
     * 第二个参数: 向第一个参数传递的地址信息的字节数,IPv4为4,IPv6为16
     * 第三个参数: 传递地址族信息,IPv4时为AF_INET,IPv6为AF_INET6
     */
    host = gethostbyaddr((char *)&addr.sin_addr,4,AF_INET);
    if (!host){
        error_handling("gethost... error");
    }
    printf("Official name: %s \n",host->h_name);
    for (i = 0;  host->h_aliases[i] ; i++) {
        printf("Aliases %d: %s \n",i + 1,host->h_aliases[i]);
    }
    printf("Address type: %s \n",(host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for (i = 0;  host->h_addr_list[i] ; i++) {
        printf("IP addr %d : %s \n",i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}