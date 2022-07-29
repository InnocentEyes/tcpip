//
// Created by qiuzelin1 on 2022/7/29.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
/**
 * epoll 轮询
 */
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(char *message);

void setnonblockingmode(int fd);

/**
 * echo_EPETserv2.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int serv_sock,clnt_sock;
    socklen_t clnt_addr_sz;
    int eqfd,event_cn;
    struct epoll_event event;
    struct epoll_event *ep_event;
    ssize_t str_len;
    char buf[BUF_SIZE];
    if (argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serv_sock == -1){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind error ~");
    }

    if(listen(serv_sock,15) == -1){
        error_handling("listen error ~");
    }
    eqfd = epoll_create(EPOLL_SIZE);
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(eqfd,EPOLL_CTL_ADD,serv_sock,&event);
    ep_event = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    while (1){
        event_cn = epoll_wait(eqfd,ep_event,EPOLL_SIZE,-1);
        if(event_cn == -1){
            puts("epoll_wait() error ~");
            break;
        }
        puts("return epoll_wait");
        for (int i = 0; i < event_cn; ++i) {
            if(ep_event[i].data.fd == serv_sock){
                clnt_addr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_sz);
                if(clnt_sock == -1){
                    error_handling("accept error ~");
                }
                /**
                 * 设置为非阻塞文件描述符
                 */
                setnonblockingmode(clnt_sock);
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(eqfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client: %d \n",clnt_sock);
            }else{
                while (1){
                    str_len = read(ep_event[i].data.fd,buf,BUF_SIZE);
                    if(str_len == 0){
                        epoll_ctl(eqfd,EPOLL_CTL_DEL,ep_event[i].data.fd,NULL);
                        close(ep_event[i].data.fd);
                        printf("closed client: %d \n",ep_event[i].data.fd);
                        continue;
                    }
                    if (str_len < 0){
                        if (errno == EAGAIN)
                            break;
                    }
                    write(ep_event[i].data.fd,buf,str_len); //echo!
                }
            }
        }
    }
    return 0;
}

void setnonblockingmode(int fd){
    int flag = fcntl(fd,F_GETFD,0);
    fcntl(fd,F_SETFD,flag | O_NONBLOCK);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}