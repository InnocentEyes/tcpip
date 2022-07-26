//
// Created by qiuzelin1 on 2022/6/24.
//
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(){
    struct sockaddr_in addr_inet;
    char *addr = "123.234.124.79";
    if (!inet_aton(addr, &addr_inet.sin_addr)){
        error_handling("Conversion error");
    }else{
        printf("Network ordered integer addr: %#x \n",addr_inet.sin_addr.s_addr);
    }
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
