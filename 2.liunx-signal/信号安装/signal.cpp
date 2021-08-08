#include <iostream>
#import <signal.h>
#import <unistd.h>
//clang++ -std=c++11 -g -o signal signal.cpp
void sighandle(int sig){
    std::cout << "receved signal:" << sig << std::endl;
}

int main(int argc, const char * argv[]) {
   
    signal(SIGINT, sighandle); //SIGINT 从键盘触发 ctr+c
    signal(SIGQUIT, sighandle); /*ctr+\ */
    signal(SIGHUP, sighandle); //关闭终端 ./signal > sig.txt
    pause();
    
    return 0;
}
