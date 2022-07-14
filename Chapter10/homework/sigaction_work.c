//
// Created by qiuzelin1 on 2022/7/14.
//

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int);

void keycontrol(int);

/**
 * ch10 task5 homework
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int i = 0;
    struct sigaction act;
    struct sigaction alarm_act;
    act.sa_handler = keycontrol;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT,&act,0);
    alarm_act.sa_handler = timeout;
    sigemptyset(&alarm_act.sa_mask);
    alarm_act.sa_flags = 0;
    sigaction(SIGALRM,&alarm_act,0);
    alarm(1);
    for (i = 0; i < 3 ; i++) {
        printf("wait....");
        sleep(100);
    }
    return 0;
}

void timeout(int sig){
    if (sig == SIGALRM){
        printf("Hi!\n");
    }
    alarm(1);
}

void keycontrol(int sig){
    if (sig == SIGINT){
        fputs("press ctrl+c !,Would you want to quit the process. y/n ? ",stdout);
        if(fgetc(stdin) == 'y'){
            exit(1);
        }
    }
    printf("\n");
}

