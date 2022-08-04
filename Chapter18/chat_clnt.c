//
// Created by qiuzelin1 on 2022/8/2.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(char *message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

/**
 * chat_clnt.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t read_id,write_id;
    void *thread_return;
    if(argc != 4){
        printf("Usage : %s <ip> <port> <name> \n",argv[0]);
        exit(1);
    }
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock == -1){
        error_handling("socket create error ~");
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect error ~");
    }
    sprintf(name,"[%s]",argv[3]);
    pthread_create(&read_id,NULL,recv_msg,&sock);
    pthread_create(&write_id,NULL,send_msg,&sock);
    pthread_join(read_id,&thread_return);
    pthread_join(write_id,&thread_return);
    close(sock);
    return 0;
}

//send thread main
void* send_msg(void *arg){
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    while (1){
        fgets(msg,BUF_SIZE,stdin);
        if(!strcmp(msg,"q\n") || !strcmp(msg,"Q\n")){
            close(sock);
            exit(0);
        }
        sprintf(name_msg,"%s %s",name,msg);
        write(sock,name_msg, strlen(name_msg));
    }
    return NULL;
}

//read thread main
void *recv_msg(void *arg){
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];
    ssize_t str_len;
    while (1){
        str_len = read(sock,name_msg,NAME_SIZE + BUF_SIZE - 1);
        if(str_len == -1){
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg,stdout);
    }
    return NULL;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

