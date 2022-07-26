//
// Created by qiuzelin1 on 2022/7/12.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define true 1
#define false 0

void error_handling(char *message);

/**
 * 使用Nagle算法 刚才说过的"大文件数据"应禁用Nagle算法。换言之,如果有必要,就应禁用Nagle算法
 * @param argc
 * @param argv
 * @return　
 */
int main(int argc,char *argv[]){
    int serv_sock;
    socklen_t optlen;
    int opt_type;
    int state;
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1){
        error_handling("socket create error!");
    }
    optlen = sizeof(opt_type);
    state = getsockopt(serv_sock,IPPROTO_TCP,TCP_NODELAY,&opt_type,&optlen);
    if (state){
        error_handling("getsocket.... error");
    }
    printf("is used nagle algo : %s",(opt_type == true) ? "no" : "yes");
    opt_type = opt_type == true ? false : true;
    optlen = sizeof(opt_type);
    state = setsockopt(serv_sock, IPPROTO_TCP, TCP_NODELAY, &opt_type, optlen);
    if (state){
        error_handling("getsocket.... error");
    }
    printf("is used nagle algo : %s",(opt_type == true) ? "no" : "yes");
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}