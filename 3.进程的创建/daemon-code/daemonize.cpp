#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/*
 clang++ -g -o daemon daemonize.cpp
 ps -ef | grep daemon
 501  2465     1   0  1:41下午 ??         0:00.00 ./daemon
 kill -9 2465
 */

void daemonize(){
    int fd;
    pid_t pid;
    
    if ((pid = fork()) < 0) {//小于0创建进程失败
        std::cout << "can't create suprocess!" << std::endl;
    }else{
        if(pid != 0){
            exit(0);//退出父进程
        }
    }
    
    setsid();//设置进程会话
    
    if(chdir("/") < 0){//改变工作目录
        std::cout << "can't change dir!" << std::endl;
        exit(-1);
    }
    
    //重定向标准输入输出和错误
    fd = open("/dev/null",O_RDWR);
    dup2(fd,STDIN_FILENO);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);
    
    
    
    
}

int main(int argc,char* argv[]){
    
    daemonize();
    
    
    while (1) {
        sleep(1);
    }
    
    return 0;
}
