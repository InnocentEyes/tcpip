//
// Created by qiuzelin1 on 2022/7/13.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_childproc(int sig);

/**
 * 多进程服务端
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    if (argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;
    pid_t pid;
    ssize_t read_len;
    char message[BUF_SIZE];
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = read_childproc;
    act.sa_flags = 0;
    sigaction(SIGCHLD,&act,0);
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (serv_sock == -1){
        error_handling("socket create error!");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind error!");
    }
    if(listen(serv_sock,15) == -1){
        error_handling("listen error!");
    }
    while (1){
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_adr_sz);
        if (clnt_sock == -1){
            continue;
        } else{
            puts("new client connected...");
        }
        pid = fork();
        if (pid == -1){
            close(clnt_sock);
            continue;
        }
        if (pid == 0){
            close(serv_sock);
            while ((read_len = read(clnt_sock,message,BUF_SIZE)) != 0){
                write(clnt_sock,message,read_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig){
    pid_t id;
    int state;
    id = waitpid(-1,&state,WNOHANG);
    printf("removed proc id: %d \n", id);
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}