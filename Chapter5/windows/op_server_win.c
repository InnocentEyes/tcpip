//
// Created by qiuzelin1 on 2022/7/7.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define INT_SIZE sizeof(int)

void error_handling(char *message);

/**
 * windows环境下的 operator client程序
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 3){
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(1);
    }
    WSADATA wsadata;
    SOCKADDR_IN servAddr;
    SOCKET sock;
    int total;
    int *p;
    int write_len;
    int *result;
    if(WSAStartup(MAKEWORD(2,2),&wsadata) != 0){
        error_handling("WSAStartup() error.");
    }
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == INVALID_SOCKET){
        error_handling("socket() error!");
    }
    memset(&servAddr,0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == SOCKET_ERROR){
        error_handling("connect() error!");
    }else{
        printf("Connected...\n");
    }
    fputs("Operand count: ",stdout);
    scanf("%d",&total);
    write_len = (INT_SIZE * (total+1)) + 1;
    p = calloc(write_len,1);
    p[0] = total;
    for (int i = 1; i <= total; ++i) {
        printf("Operand %d: ",i);
        scanf("%d",&p[i]);
    }
    fgetc(stdin);
    printf("Operation: ");
    scanf("%c",&(((char *)p)[write_len - 1]));
    printf("writing...\n");
    send(sock, (const char *) p, write_len, 0);
    recv(sock, (char *) result, INT_SIZE, 0);
    printf("recv operator result: %d",*result);
    closesocket(sock);
    free(p);
    WSACleanup();
    return 0;
}

void erro_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}