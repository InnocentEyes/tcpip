//
// Created by qiuzelin1 on 2022/7/7.
//
#include <stdio.h>
#include <stdlib.h>

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}