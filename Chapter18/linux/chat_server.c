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
#define MAX_CLNT 256

void* handle_clnt(void* arg);
void send_msg(char* msg,ssize_t len);
void error_handling(char *message);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

/**
 * chat_server.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;
    pthread_t t_id;
    if(argc != 2){
        printf("Usage : %s <port> \n",argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex,NULL);
    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_adr,0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (const struct sockaddr *) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock,5) == -1){
        error_handling("listen() error");
    }

    while (1){
        fputs("stop? y/n",stdout);
        int opera = fgetc(stdin);
        if(opera == 'y'){
            break;
        }
        clnt_adr_sz = sizeof(clnt_adr);
        printf("accepting...\n");
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id,NULL,handle_clnt,&clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    return 0;
}


void* handle_clnt(void* arg){
    int clnt_sock = *((int*)arg);
    ssize_t str_len = 0,i;
    char msg[BUF_SIZE];

    while ((str_len = read(clnt_sock,msg, sizeof(msg))) != 0){
        send_msg(msg,str_len);
    }
    pthread_mutex_lock(&mutex);
    for (i = 0; i < clnt_cnt; i++) { // remove disconnected client
        if(clnt_sock == clnt_socks[i]){
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}

//send to all
void send_msg(char* msg,ssize_t len){
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < clnt_cnt; i++) {
        write(clnt_socks[i],msg,len);
    }
    pthread_mutex_unlock(&mutex);
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
