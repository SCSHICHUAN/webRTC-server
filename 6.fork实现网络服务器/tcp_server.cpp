#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 8888
#define MESSAGE_SIZE 1024
int main(int argc,char* argv[]){
   
   int ret = -1;

   int socket_fd = -1;
   int accept_fd = -1;

   int curpos = 0;
   int backlog = 10;//同一时间链接的socket数量，毫秒级别的同时链接入
   int flag = 1;
   pid_t pid;
   
   char in_buf[MESSAGE_SIZE] = {0,};

   
   struct sockaddr_in local_addr,remote_addr;
   
   //创建socket
   socket_fd = socket(AF_INET,SOCK_STREAM,0);//如果有疑问可以shift+K查看帮助文档
   if(socket_fd == -1){
      std::cout << "Failed to create scocket!" << std::endl;
   }  
  
   //设置端口地址可以重用，on起作用
   ret =  setsockopt(socket_fd, 
              SOL_SOCKET,//level
              SO_REUSEADDR,
              &flag,
              sizeof(flag));
   
   if(ret == -1){
       std::cout << "Failed to create socket options!" << std::endl;
   }

    //set local address
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = PORT;
    local_addr.sin_addr.s_addr = INADDR_ANY;//0监听所有网卡，都启动服务
    bzero(&(local_addr.sin_zero),8);

    
    //bind socket
    ret = bind(socket_fd,
               (struct sockaddr *)&local_addr,//指针转换
                sizeof(struct sockaddr_in));
    if(ret == -1){
       perror("bind errot!");
       exit(1);
    }

    //listen    
    ret = listen(socket_fd,backlog);
    if(ret == -1){
       perror("listen error!");
       exit(1);
    }


    //loop
    for(;;){
      int addr_len = sizeof( struct sockaddr_in );
      //accept an new connection
      accept_fd = accept( socket_fd, (struct sockaddr *)&remote_addr, (socklen_t*)&addr_len );
        
        //创建一个新的进程来接收cline socket
        pid = fork();
        if (pid == 0) {//“== 0”什么是子进程
            while(1){
                memset(in_buf,0,MESSAGE_SIZE);

               //receive network data and print buf
               ret = recv(accept_fd,(void*)in_buf,MESSAGE_SIZE,0);
               if(ret <= 0){
                  break;
               }
             
              printf("接收:%s\n",in_buf);
              ret =  send(accept_fd,(void*)in_buf,MESSAGE_SIZE,0);
            }
          close(accept_fd);
          printf("close client connection...\n");
            
        }
    }

    if (pid != 0) {
        printf("quit server ....\n");
        close(socket_fd);
    }
   


   return 0;
}
