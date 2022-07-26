//
// Created by qiuzelin1 on 2022/7/6.
//
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

void error_handling(char *message);

int operation(int total, int *p, char oper);

/**
 * operater server
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    char *p;
    int serv_sock, clnt_sock;
    socklen_t clnt_addr_sz;
    ssize_t read_len;
    ssize_t recv_len = 0;
    int result = 0;
    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        error_handling("socket() error.");
    }
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error!");
    }
    clnt_addr_sz = sizeof(clntAddr);
    printf("start working...\n");
    for (int i = 0; i < 5; ++i) {
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clntAddr, &clnt_addr_sz);
        printf("client %d clnt_sock : %d\n", i, clnt_sock);
        if (clnt_sock == -1) {
            error_handling("accept() error!");
        } else {
            printf("Client Connected %d\n", i);
        }
        int total = 0;
        printf("start reading...\n");
        read(clnt_sock, &total, sizeof(int));
        printf("total : %d\n", total);
        read_len = (4 * total) + 1;
        p = malloc(read_len);
        while (recv_len < read_len) {
            recv_len += read(clnt_sock, &p[recv_len], 4);
        }
        result = operation(total, (int *) p, p[recv_len - 1]);
        printf("push operator result : %d\n",result);
        write(clnt_sock, &result, sizeof(int));
        recv_len = 0;
        printf("----------\n");
        free(p);
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int operation(int total, int *p, char oper) {
    int result = 0;
    switch (oper) {
        case '+':
            for (int i = 0; i < total; ++i) {
                result += *p;
                p++;
            }
            break;
        case '-':
            for (int i = 0; i < total; ++i) {
                result -= *p;
                p++;
            }
            break;
        case '*':
            for (int i = 0; i < total; ++i) {
                result *= *p;
                p++;
            }
            break;
        case '/':
            for (int i = 0; i < total; ++i) {
                result /= *p;
                p++;
            }
            break;
    }
    return result;
}
