//
// Created by qiuzelin1 on 2022/7/7.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include "error_handler.h"

int arithmetic(int total,int a[],char operator);

/**
 * windows环境下的 server
 * @return
 */
#define INT_SIZE sizeof(int)
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }
    SOCKET serv_sock;
    SOCKET clnt_sock;
    SOCKADDR_IN clnt_addr;
    SOCKADDR_IN serv_addr;
    WSADATA wsadata;
    int clnt_add_sz;
    int read_len;
    char *p;
    int recv_len = 0;
    int result;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        error_handling("WSAStartup() error!");
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET) {
        error_handling("socket() error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR){
        error_handling("bind() error!");
    }
    if (listen(serv_sock,5) == SOCKET_ERROR){
        error_handling("listen() error!");
    }
    clnt_add_sz = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_add_sz);
        if(clnt_sock == SOCKET_ERROR){
            error_handling("accept() error!");
        }
        int total = 0;
        recv(clnt_sock,(char *)(&total),INT_SIZE,0);
        read_len = (INT_SIZE * total) + 1;
        p = calloc(read_len,1);
        while (recv_len < read_len){
            recv_len += recv(clnt_sock,p,INT_SIZE,0);
        }
        result = arithmetic(total,(int *)p,p[read_len - 1]);
        printf("push operator result %d",result);
        send(clnt_sock,(char *)(&result),INT_SIZE,0);
        free(p);
        recv_len = 0;
        closesocket(clnt_sock);
    }
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

int arithmetic(int total,int a[],char operator){
    int result = a[0];
    switch (operator) {
        case '+':
            for (int i = 1; i < total; ++i) {
                result += a[i];
            }
            break;
        case '-':
            for (int i = 0; i < total; ++i) {
                result -= a[i];
            }
            break;
        case '*':
            for (int i = 1; i < total; ++i) {
                result *= a[i];
            }
            break;
        case '/':
            for (int i = 1; i < total; ++i) {
                result /= a[i];
            }
            break;
    }
    return result;
}
