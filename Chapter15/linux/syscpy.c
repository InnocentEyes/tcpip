//
// Created by qiuzelin1 on 2022/7/26.
//

#include <stdio.h>
#include <fcntl.h>
#define BUF_SIZE 3 //用最短数组长度构成

/**
 * linux环境下调用系统函数复制文件内容
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int fd1,fd2; //保存在fd1和fd2中的文件描述符
    int len;
    char buf[BUF_SIZE];
    fd1 = open("news.txt",O_RDONLY);
    fd2 = open("cpy.txt",O_WRONLY | O_CREAT | O_TRUNC);
    while ((len = read(fd1,buf,BUF_SIZE)))
        write(fd2,buf, strlen(buf));
    close(fd1);
    close(fd2);
    return 0;
}


