//
// Created by qiuzelin1 on 2022/7/29.
//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void *arg);

/**
 * 创建线程 thread1.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    pthread_t t_id;
    int thread_param = 5;
    if(pthread_create(&t_id,NULL,thread_main,&thread_param) != 0){
        puts("pthread_create() error");
        return -1;
    }
    sleep(10);
    puts("end of main");
    return 0;
}

void* thread_main(void *arg){
    int i;
    int cnt = *((int *)arg);
    for (i = 0;  i < cnt ; i++) {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}