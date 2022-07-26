//
// Created by qiuzelin1 on 2022/7/6.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);

/**
 * operater client
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage : %s <ip> <port>\n", argv[0]);
        exit(1);
    }
    int sock;
    struct sockaddr_in servAddr;
    int *p;
    int write_len;
    int member = 0;
    int result = 0;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        error_handling("socket() error!");
    }
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (const struct sockaddr *) &servAddr, sizeof(servAddr)) == -1) {
        error_handling("connect() error!");
    } else {
        printf("Connected...\n");
    }
    int total = 0;
    fputs("Operand count: ", stdout);
    scanf("%d", &total);
    write_len = (4 * (total + 1)) + 1;
    p = malloc(write_len);
    p[0] = total;
    for (int i = 1; i <= total; ++i) {
        printf("Operand %d :", i);
        scanf("%d", &member);
        p[i] = member;
    }
    fgetc(stdin);
    fputs("Operation :", stdout);
    scanf("%c", &(((char *) p)[write_len - 1]));
    printf("writing ...\n");
    write(sock, p, write_len);
    read(sock, &result, sizeof(int));
    printf("recv operator result : %d\n", result);
    free(p);
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


