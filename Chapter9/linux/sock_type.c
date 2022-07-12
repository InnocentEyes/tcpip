//
// Created by qiuzelin1 on 2022/7/11.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int udp_sock;
    int tcp_sock;
    socklen_t optlen;
    int sock_type;
    int state;
    udp_sock = socket(PF_INET,SOCK_DGRAM,0);
    tcp_sock = socket(PF_INET,SOCK_STREAM,0);
    printf("sock stream : %d\n",SOCK_STREAM);
    printf("sock dgram : %d\n",SOCK_DGRAM);
    optlen = sizeof(tcp_sock);
    state = getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,&sock_type,&optlen);
    if(state){
        error_handling("getsockopt() error!");
    }
    printf("Socket type one: %d \n",sock_type);
    state = getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,&sock_type,&optlen);
    if(state){
        error_handling("getsockopt() error!");
    }
    printf("Socket type two: %d \n",sock_type);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
