//
// Created by qiuzelin1 on 2022/8/2.
//

#include <windows.h>
#include <stdio.h>
#include <process.h>
unsigned WINAPI Read(void *arg);
unsigned WINAPI Accu(void *arg);


static HANDLE semOne;
static HANDLE semTwo;
static int num;

/**
 * semaphore_win.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    HANDLE hThread1,hThread2;
    semOne = CreateSemaphoreA(NULL,0,1,NULL);
    semTwo = CreateSemaphoreA(NULL,1,1,NULL);

    hThread1 = (HANDLE) _beginthreadex(NULL,0,Read,NULL,0,NULL);
    hThread2 = (HANDLE) _beginthreadex(NULL,0,Accu,NULL,0,NULL);

    WaitForSingleObject(hThread1,INFINITE);
    WaitForSingleObject(hThread2,INFINITE);
    CloseHandle(semOne);
    CloseHandle(semTwo);
    return 0;
}

unsigned WINAPI Read(void *arg){
    int i;
    for (i = 0; i < 5 ; i++) {
        fputs("Input num: ",stdout);
        WaitForSingleObject(semTwo,INFINITE);
        scanf("%d",&num);
        ReleaseSemaphore(semOne,1,NULL);
    }
}

unsigned WINAPI Accu(void *arg){
    int sum = 0,i;
    for (i = 0; i < 5; i++) {
        WaitForSingleObject(semOne,INFINITE);
        sum += num;
        ReleaseSemaphore(semTwo,1,NULL);
    }
    printf("Result: %d \n",sum);
    return 0;
}