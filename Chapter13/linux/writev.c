//
// Created by qiuzelin1 on 2022/7/22.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>


/**
 * writev.c
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";
    ssize_t str_len;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;
    str_len = writev(1,vec,2);
    puts("");
    printf("Write bytes: %zd \n",str_len);
    return 0;
}
