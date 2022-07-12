//
// Created by qiuzelin1 on 2022/7/11.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void error_handling(char *message);

/**
 * file_client.c 半连接客户端
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_adr;
    if (argc != 3){
        printf("Usage: %s <ip> <port> \n",argv[0]);
        exit(1);
    }
    fp = fopen("receive.dat","wb");
    sd = socket(PF_INET,SOCK_STREAM,0);
    if (sd == -1){
        error_handling("socket() error!");
    }
    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htonl(atoi(argv[2]));
    if(connect(sd, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error!");
    }
    while ((read_cnt = read(sd,buf,BUF_SIZE)) != 0){
        fwrite(buf,1,read_cnt,fp);
    }
    puts("Received file data");
    write(sd,"Thank you",10);
    fclose(fp);
    close(sd);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
