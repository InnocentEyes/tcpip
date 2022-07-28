//
// Created by qiuzelin1 on 2022/7/27.
//
#include <stdio.h>
#include <fcntl.h> //linux内核提供的文件操作的api

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(void){
    FILE *fp;
    int fd;
    fd = open("data.bat",O_WRONLY | O_CREAT | O_TRUNC);
    if(fd == -1){
        printf("error ~ \n");
        return -1;
    }
    printf("First file descriptor: %d \n",fd);
    fp = fdopen(fd,"w");
    fputs("TCP/IP SOCKET Programming... \n",fp);
    printf("Second file descriptor: %d \n", fileno(fp));
    fclose(fp);
    return 0;
}