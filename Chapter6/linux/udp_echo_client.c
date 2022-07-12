//
// Created by qiuzelin1 on 2022/7/7.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port>\n",argv[0]);
        exit(1);
    }
    struct sockaddr_in serv_addr,from_addr;
    int sock;
    char message[BUF_SIZE];
    socklen_t adr_sz;
    ssize_t str_len;
    sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock == -1){
        error_handling("socket() error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    while (1){
        fputs("Insert message(q to quit): ",stdout);
        fgets(message, sizeof(message),stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n")){
            break;
        }
        sendto(sock, message, strlen(message), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        adr_sz = sizeof(from_addr);
        str_len = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &from_addr, &adr_sz);
        message[str_len] = '\0';
        printf("Message from server : %s",message);
    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}