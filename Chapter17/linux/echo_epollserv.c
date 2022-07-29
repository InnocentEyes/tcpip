//
// Created by qiuzelin1 on 2022/7/28.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
/**
 * epoll 轮询
 */
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(char *message);

/**
 * echo_epollserv.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_sock_sz;
    struct epoll_event *ep_event;
    struct epoll_event event;
    int epfd,event_cnt;
    char buf[BUF_SIZE];
    ssize_t str_len;
    if(argc != 2){
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

    if(listen(serv_sock,5) == -1){
        error_handling("listen error ~");
    }
    epfd = epoll_create(EPOLL_SIZE);
    event.events = EPOLLIN; //
    event.data.fd = serv_sock; // 设置套接字
    ep_event = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    epoll_ctl(epfd,EPOLL_CTL_ADD,serv_sock,&event);
    /**
     * 开始做处理~
     */
    while (1){
        event_cnt = epoll_wait(epfd,ep_event,EPOLL_SIZE,-1);
        if(event_cnt == -1){
            puts("epoll_wait() error");
            break;
        }
        for (int i = 0; i < event_cnt; ++i) {
            if(ep_event[i].data.fd == serv_sock){
                clnt_sock_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_sock_sz);
                if(clnt_sock == -1){
                    error_handling("accpet error ~");
                }
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client: %d \n",clnt_sock);
            }else{
                str_len = read(ep_event[i].data.fd,buf,BUF_SIZE);
                if(str_len < 0){
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_event[i].data.fd,NULL);
                    close(ep_event[i].data.fd);
                    printf("closed client: %d \n",ep_event[i].data.fd);
                    continue;
                }
                write(ep_event[i].data.fd,buf,str_len); //echo !
            }
        }
    }
    close(serv_sock);
    close(epfd);
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
