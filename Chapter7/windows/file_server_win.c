//
// Created by qiuzelin1 on 2022/7/11.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>

#define BUF_SIZE 30

void error_handling(char *message);


/**
 * windows环境下的 半连接 服务端
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }
    SOCKET serv_sock;
    SOCKET clnt_sock;
    SOCKADDR_IN serv_addr;
    SOCKADDR_IN clnt_addr;
    int clnt_addr_sz;
    int read_cnt;
    FILE *fp;
    char buf[BUF_SIZE];
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        error_handling("WSAStartup() error!");
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET) {
        error_handling("socket() error!");
    }
    fp = fopen("file_server_win.c", "rb");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5) == SOCKET_ERROR) {
        error_handling("listen() error!");
    }
    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_sz);
    while (1) {
        read_cnt = fread(buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            send(clnt_sock, buf, read_cnt, 0);
            break;
        }
        send(clnt_sock,buf,BUF_SIZE,0);
    }
    shutdown(clnt_sock,SD_SEND);
    recv(clnt_sock,buf,BUF_SIZE,0);
    printf("Message from client %s \n",buf);
    fclose(fp);
    closesocket(clnt_sock);
    closesocket(serv_sock);
    WSACleanup();
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
