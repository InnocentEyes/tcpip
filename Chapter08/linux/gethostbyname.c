//
// Created by qiuzelin1 on 2022/7/11.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void error_handling(char *message);

/**
 *
 * struct hostent{
 *   char * h_name;     //official name
 *   char ** h_aliases; // alias list
 *   int h_addrtype; // host address type
 *   int h_length; // address length
 *   char ** h_addr_list; // address list
 * }
 *
 * @param agrc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    struct hostent *host;
    if (argc != 2){
        printf("Usage : %s <addr>\n",argv[0]);
        exit(1);
    }
    host = gethostbyname(argv[1]);
    if (!host){
        error_handling("gethost.... error");
    }
    printf("Official name: %s \n",host->h_name);
    for (int i = 0; host->h_aliases[i]; ++i) {
        printf("Aliases %d: %s \n",i + 1,host->h_aliases[i]);
    }
    printf("Address type: %s \n",(host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for (int i = 0; host->h_addr_list[i]; ++i) {
        printf("IP addr %d: %s \n",i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

