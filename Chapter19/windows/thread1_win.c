//
// Created by qiuzelin1 on 2022/8/2.
//

#include <stdio.h>
#include <windows.h>
#include <process.h> /* _beginthreadex, _endthreadex */

unsigned WINAPI ThreadFunc(void* arg);



/**
 * thread1_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    HANDLE hThread; //CreateThread函数返回值类型HANDLE
    unsigned threadID;
    int param = 5;

    hThread = (HANDLE) _beginthreadex(NULL,0,ThreadFunc,&param,0,&threadID);
    if(hThread == 0){
        puts("_beginthreadex() error");
        return -1;
    }
    sleep(3000);
    puts("end of main");
    return 0;
}

unsigned WINAPI ThreadFunc(void *arg){
    int i;
    int cnt = *((int*)arg);
    for (i = 0; i < cnt ; i++) {
        Sleep(1000);
        puts("running thread");
    }
    return 0;
}