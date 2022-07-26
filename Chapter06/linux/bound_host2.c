//
// Created by qiuzelin1 on 2022/7/8.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);

/**
 *
 * @return
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }
    int sock;
    struct sockaddr_in your_addr;
    ssize_t adr_sz;
    char *message1 = "Hi!";
    char *message2 = "I'm another UDP host.";
    char *message3 = "Nice to meet you.";
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == -1) {
        error_handling("socket() create error!");
    }
    memset(&your_addr, 0, sizeof(your_addr));
    your_addr.sin_family = AF_INET;
    your_addr.sin_addr.s_addr = inet_addr(argv[1]);
    your_addr.sin_port = htonl(atoi(argv[2]));
    adr_sz = sizeof(your_addr);
    sendto(sock, message1, strlen(message1), 0, (const struct sockaddr *) &your_addr, adr_sz);
    sendto(sock, message2, strlen(message2), 0, (const struct sockaddr *) &your_addr, adr_sz);
    sendto(sock, message3, strlen(message3), 0, (const struct sockaddr *) &your_addr, adr_sz);
    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
