//
// Created by qiuzelin1 on 2022/7/11.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(char *message);

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int sock;
    int snd_buf,rcv_buf,state;
    socklen_t len;

    sock = socket(PF_INET,SOCK_STREAM,0);
    len = sizeof(snd_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&snd_buf,&len);
    if (state){
        error_handling("getsockopt() error");
    }
    len = sizeof(rcv_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&rcv_buf,&len);
    if (state){
        error_handling("getsockopt() error");
    }
    printf("Input buffer size: %d \n",rcv_buf);
    printf("Output buffer size: %d \n",snd_buf);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
