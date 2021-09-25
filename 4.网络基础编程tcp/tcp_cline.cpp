#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

#define PROT 8888
#define MESSAGE_SIZE 1024
int main(int argc,char*argv[]){
    
    int socket_fd;
    struct sockaddr_in serveraddr;
    
    int ret = -1;
    
    char sendbuf[MESSAGE_SIZE] = {0,};
    char reviceBuf[MESSAGE_SIZE] = {0,};
    
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if (socket_fd < 0) {
        std::cout<< "Faileed to creat socket!" <<std::endl;
        exit(-1);
    }
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = PROT;
    serveraddr.sin_addr.s_addr = inet_addr("192.168.1.6");
    
    ret = connect(socket_fd,
                     (struct sockaddr*)&serveraddr,
                 sizeof(struct sockaddr));
    if(ret < 0){
        std::cout<< "Faileed to cnnect sever!" <<std::endl;
    }else{
        std::cout<< "success to cnnect sever!" <<std::endl;
    }
    
    while (1) {
        memset(sendbuf,0,MESSAGE_SIZE);
        std::cout<<""<<std::endl;
        std::cout<< "请输入:" <<std::endl;
        
        gets(sendbuf);
        ret =  send(socket_fd,sendbuf,strlen(sendbuf),0);
        if (ret <= 0) {
            printf("the connection is disconnection!\n");
            break;
        }
        if (strcmp(sendbuf,"quit") == 0) {
            break;
        }
       
        reviceBuf[0] = '\0';
        ret = recv(socket_fd,reviceBuf,MESSAGE_SIZE,0);
        reviceBuf[ret] = '\0';
        std::cout << "接收:" << std::endl << reviceBuf << std::endl;
        
    }
    
    close(socket_fd);
    
    return 0;
}
