//
// Created by qiuzelin1 on 2022/7/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);

/**
 * new_receiver.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int recv_sock;
    ssize_t str_len;
    struct sockaddr_in adr;
    char buf[BUF_SIZE];
    struct ip_mreq join_adr;
    if(argc != 3){
        printf("Usage : %s <GoupIP> <PORT> \n",argv[0]);
        exit(1);
    }
    recv_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(recv_sock == -1){
        error_handling("socket create error~");
    }
    memset(&adr,0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[2]));
    if(bind(recv_sock, (const struct sockaddr *) &adr, sizeof(adr)) == -1){
        printf("bind address : %u error~ \n",adr.sin_addr.s_addr);
        exit(1);
    }
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
    join_adr.imr_multiaddr.s_addr = htonl(atoi(argv[1]));
    setsockopt(recv_sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&join_adr, sizeof(join_adr));
    while(1){
        str_len = recvfrom(recv_sock,buf,BUF_SIZE,0,NULL,0);
        if(str_len < 0){
            break;
        }
        buf[str_len] = 0;
        fputs(buf,stdout);
    }
    close(recv_sock);
    return 0;
}

/**
 * 错误处理
 * @param message
 */
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}