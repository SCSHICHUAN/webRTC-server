#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
    if (daemon(0,0) == -1) {
        std::cout << "errot" << std::endl;
    }
    
    while (1) {
        sleep(1);
    }
}
