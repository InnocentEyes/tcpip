//
// Created by qiuzelin1 on 2022/6/23.
//

//在linux环境中使用 ASNI标准提供的I/O 和 操作系统提供的文件I/O的文件复制程序
#include <stdio.h>

#include <stdlib.h>

void rewrtie(char *p){
}


int main(){
    FILE *readFile;
    readFile = fopen("data.txt","rb+");
    fseek(readFile,0,SEEK_END);
    long size = ftell(readFile);
    char *buf = (char*)malloc(size);
    rewind(readFile);
    fgets(buf,(int)size,readFile);
    printf("read file is %s\n",buf);
    FILE *writeFile;
    writeFile = fopen("test.dat","wb+");
    fputs(buf,writeFile);
    fclose(writeFile);
    fclose(readFile);
    free(buf);
    return 0;
}

