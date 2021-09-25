#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

int main(int argc,char** argv){
    
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9876);
    addr.sin_addr.s_addr = inet_addr("192.168.1.6");
    
    
    
    int sock;
    int n = 0;
    socklen_t len = sizeof(addr);
    char send_buff[BUFF_SIZE];
    char recv_buff[BUFF_SIZE];
    
    //绑定地址和端口
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if (sock < 0) {
        printf("Failed to create socket!\n");
    }
    
    if (addr.sin_addr.s_addr == INADDR_NONE) {
        printf("Incorrect IP address!\n");
        close(sock);
        exit(1);
    }
    
    
    //发送和接收
    while (1) {
        memset(send_buff,0,BUFF_SIZE);
        printf("\n\nWaiting for input...\n");
        gets(send_buff);
        
        n = sendto(sock,send_buff,strlen(send_buff),
                   0,(struct sockaddr*)&addr,
                   sizeof(addr));
        if (n > 0) {
            printf("Successed send meddage!\n");
        }
        
        n = recvfrom(sock,recv_buff,BUFF_SIZE,0,
                    (struct sockaddr *)&addr,&len);
        if (n > 0) {
            printf("server:%s",recv_buff);
        }
        
    }
    
    close(sock);
    return 0;
}
