//
// Created by qiuzelin1 on 2022/7/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 100

void error_handling(char *message);

void read_childProc(int sig);

/**
 * 改进echo_mpserv服务端,服务端在收到消息时写入
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    pid_t pid;
    int fds[2];
    ssize_t str_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    struct sigaction act;
    socklen_t clnt_addr_sz;
    char buf[BUF_SIZE];
    serv_sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (serv_sock == -1){
        error_handling("socket create error!");
    }
    if (argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind error!");
    }
    act.sa_handler = read_childProc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD,&act,0);
    if(listen(serv_sock,15) == -1){
        error_handling("listen error!");
    }
    pipe(fds);
    pid = fork();
    if (pid == 0){
        /* 这里应该关闭吧 */
        close(serv_sock);
        FILE *fp = fopen("./echo_serv.log","wt");
        char msgbuf[BUF_SIZE];
        for (int i = 0; i < 10; ++i) {
            ssize_t len = read(fds[0],msgbuf,BUF_SIZE);
            fwrite(msgbuf,1,len,fp);
        }
        fclose(fp);
        return 0;
    }

    while (1){
        clnt_addr_sz = sizeof(clnt_addr);
        clnt_sock =  accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_sz);
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
            while ((str_len = read(clnt_sock,buf,BUF_SIZE)) != 0){
                write(fds[1],buf,str_len);
                write(clnt_sock,buf,str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}

void read_childProc(int sig){
    int state;
    pid_t pid;
    if(sig == SIGCHLD){
        pid = waitpid(-1,&state,WNOHANG);
        printf("child %d process killed.\n", WEXITSTATUS(state));
    }
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
