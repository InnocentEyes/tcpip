//
// Created by qiuzelin1 on 2022/7/26.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(char *message);

/**
 * new_sender.c service
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int send_sock;
    struct sockaddr_in mul_adr;
    int time_live = TTL;
    FILE *fp;
    char buf[BUF_SIZE];
    if(argc != 3){
        printf("Usage : %s <GoupIP> <PORT> \n",argv[0]);
        exit(1);
    }
    send_sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(send_sock == -1){
        error_handling("socket create error~");
    }
    memset(&mul_adr,0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]); //Multicast IP
    mul_adr.sin_port = htons(atoi(argv[2])); //Multicast Port

    /**
     * 套接字选项
     */
    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,&time_live, sizeof(time_live));
    if((fp = fopen("news.txt","r")) == NULL){
        error_handling("fopen() error");
    }
    while (!feof(fp)){
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock, buf, strlen(buf), 0, (const struct sockaddr *) &mul_adr, sizeof(mul_adr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

