//
// Created by qiuzelin1 on 2022/7/12.
//

#include <stdio.h>
#include <unistd.h>


/**
 * linux平台下的多进程服务器
 * fork
 * @param argc
 * @param argv
 * @return
 */
int gval = 10;

int main(int argc,char *argv[]){
    pid_t pid;
    int lval = 20;
    gval++,lval+=5;
    pid = fork();
    if(pid == 0){
        gval+=2,lval+=2;
    }else{
        gval-=2,lval-=2;
    }
    if(pid == 0){
        printf("Child Proc: [%d,%d] \n",gval,lval);
    }
    else{
        printf("Parent Proc: [%d,%d] \n",gval,lval);
    }
    return 0;
}
