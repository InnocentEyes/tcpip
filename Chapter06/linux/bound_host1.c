//
// Created by qiuzelin1 on 2022/7/8.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

/**
 * bound host1
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    int serv_sock;
    ssize_t str_len;
    char message[BUF_SIZE];
    serv_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (serv_sock == -1){
        error_handling("socket() error.");
    }
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error.");
    }
    for (int i = 0; i < 3; ++i) {
        sleep(5);
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr *) &clnt_adr, &clnt_adr_sz);
        message[str_len] = '\0';
        printf("Message %d: %s \n",i + 1,message);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

