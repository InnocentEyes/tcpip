//
// Created by qiuzelin1 on 2022/7/8.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUF_SIZE 30
void error_handling(char *message);

/**
 * 半连接服务端
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int ser_sd,clnt_sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t  clnt_adr_sz;
    if(argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    fp = fopen("file_server.c","rb");
    ser_sd = socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if(bind(ser_sd, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error!");
    }
    if(listen(ser_sd,5) == -1){
        error_handling("listen() errorr!");
    }
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(ser_sd, (struct sockaddr *) &clnt_adr, &clnt_adr_sz);
    while (1){
        read_cnt = fread((void *)buf,1,BUF_SIZE,fp);
        if (read_cnt < BUF_SIZE){
            write(clnt_sd,buf,BUF_SIZE);
            break;
        }
        write(clnt_sd,buf,read_cnt);
    }
    shutdown(clnt_sd,SHUT_WR);
    read(clnt_sd,buf,BUF_SIZE);
    printf("Message from client: %s \n",buf);
    fclose(fp);
    close(clnt_sd);
    close(ser_sd);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}