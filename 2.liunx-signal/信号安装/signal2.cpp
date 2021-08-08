#include <iostream>
#include <signal.h>
#include <unistd.h>

void sighandel(int sig)
{
    std::cout <<"received signal:"<< sig <<std::endl;
}

int main(int argc,char* arg[]){
    
    struct sigaction act,oact;
    
    act.sa_handler = sighandel;
    sigfillset(&act.sa_mask);//所有的位置1，所有的信号不接收
    act.sa_flags = 0;
    
    sigaction(SIGINT,&act,&oact);
    
    pause();
    
    
    return 0;
}
