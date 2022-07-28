//
// Created by qiuzelin1 on 2022/7/27.
//


#include <stdio.h>

#define BUF_SIZE 3 //最短数组长度

/**
 * stdcpy.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    FILE *fd1,*fd2;
    char buf[BUF_SIZE];
    fd1 = fopen("news.txt","r");
    fd2 = fopen("cpy.txt","w");
    while (!feof(fd1)){
        fgets(buf,BUF_SIZE,fd1);
        fputs(buf,fd2);
    }
    fclose(fd1);
    fclose(fd2);
    return 0;
}
