//
// Created by qiuzelin1 on 2022/7/14.
//

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc,char *argv[]){
    int fds[2],fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your Message.";
    pid_t pid;
    char buf[BUF_SIZE];
    pipe(fds);
    pipe(fds2);
    pid = fork();
    if (pid == 0){
        write(fds[1],str1, sizeof(str1));
        read(fds2[0],buf,BUF_SIZE);
        printf("Child proc output : %s \n",buf);
    }
    else{
        read(fds[0],buf,BUF_SIZE);
        printf("Parent proc output : %s \n",buf);
        write(fds2[1],str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}
