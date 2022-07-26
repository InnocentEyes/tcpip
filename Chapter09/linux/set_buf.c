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
    int sock;
    int snd_buf = 1024 * 3,rcv_buf = 1024 * 3;
    int state;
    socklen_t len;
    sock = socket(PF_INET,SOCK_STREAM,0);
    state = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcv_buf, sizeof(rcv_buf));
    if (state){
        error_handling("SO_RCVBUF setsockopt() error!");
    }
    state = setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&snd_buf, sizeof(snd_buf));
    if(state){
        error_handling("SO_SNDBUF setsockopt() error!");
    }
    len = sizeof(snd_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&rcv_buf,&len);
    if(state){
        error_handling("SO_SNDBUF getsockopt() error!");
    }
    len = sizeof(rcv_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&snd_buf,&len);
    if(state){
        error_handling("SO_RCVBUF getsockopt() error!");
    }
    printf("Input buffer size : %d \n",rcv_buf);
    printf("Output buffer size : %d \n",snd_buf);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}