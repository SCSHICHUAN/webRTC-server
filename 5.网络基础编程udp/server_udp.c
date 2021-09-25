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


int main(int argc,char* *argv){
    
    
    struct sockaddr_in  addr;
    
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(9876);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    struct sockaddr_in clinet_addr;
    socklen_t len = sizeof(clinet_addr);
    
    int ret = -1;
    int sock;
    int recv_len;
    
    char recv_buff[BUFF_SIZE];
    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        printf("Failed to create socket!");
        exit(1);
    }
    
    //socket 和地址端口绑定
    ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if (ret < 0) {
        printf("Failed to bind socket to addr !");
        exit(1);
    }
    
    //接收和处理
    while (1) {
        
        recv_len = recvfrom(sock,recv_buff,BUFF_SIZE,0,
                            (struct sockaddr *)&clinet_addr,
                            &len);
        if (recv_len > 0) {
            recv_buff[recv_len] = 0;
            printf("\nclinet addr:%s\n",inet_ntoa(clinet_addr.sin_addr));
            printf("clinet port:%u\n",ntohs(clinet_addr.sin_port));
            printf("clinet message:%s\n",recv_buff);
        }
        
        recv_len = sendto(sock,recv_buff,recv_len,
                          0,(struct sockaddr*)&clinet_addr,
                          sizeof(clinet_addr));
        if (recv_len < 0) {
            printf("Failed send to clinet!");
        }
        
        
    }
    
    return 0;
}
