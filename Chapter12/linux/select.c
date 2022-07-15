//
// Created by qiuzelin1 on 2022/7/14.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

/**
 * select.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    /* fd_sed 存的是套接字描述符 */

    /**
     * struct timeval
     * {
     *      long tv_sec; //seconds
     *      long tv_usec; //microseconds
     * }
     */
    /*
        由于发生变化的文件描述符在fd_set中不会改变,但是没有发送变化的文件描述符会从1变为0
     */
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    /*
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    */
    while (1){
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1,&temps,0,0,&timeout);
        if(result == -1){
            puts("select() error!");
            break;
        }
        else if(result == 0){
            puts("Time-out!");
        }
        else
        {
            if(FD_ISSET(0,&temps)){
                str_len = read(0,buf,BUF_SIZE);
                buf[str_len] = '\0';
                printf("message from console: %s",buf);
            }
        }
    }
    return 0;
}