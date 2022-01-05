#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>

#define PORT 8888
#define MESSAGE_SIZE 1024
#define FD_SIZE 1024
int main(int argc,char* argv[]){
    
    
    int ret = -1;
    
    pid_t pid;
    int socket_fd = -1;
    int accept_fd = -1;
    int accept_fds[FD_SIZE] = {-1,};//存储接受socket对序号
    
    int curpos = -1;//找到数据accept_fds空巢的标记
    int maxpos = 0; //已经接受的socket链接的个数
    int backlog = 10;//同一时间链接的socket数量，毫秒级别的同时链接入
    int flag = 1; //打开端口重用
    
    int max_fd = -1;//accept_fds 最大的socket的值，估计是for循环的边界值
    int events = 0;//socket触发的事件
    fd_set fd_sets;//存储会发生事件的对象的容器
    
    char in_buf[MESSAGE_SIZE] = {0,};
    
    
    
    struct sockaddr_in local_addr,remote_addr;
    
    //创建 tcp socket
    socket_fd = socket(AF_INET,SOCK_STREAM,0);//如果有疑问可以shift+K查看帮助文档
    if(socket_fd == -1){
        std::cout << "Failed to create scocket!" << std::endl;
    }
    
    //设置socket option  设置端口地址可以重用，on起作用
    ret =  setsockopt(socket_fd,
                      SOL_SOCKET,//level
                      SO_REUSEADDR,
                      &flag,
                      sizeof(flag));
    if(ret == -1){
        std::cout << "Failed to create socket options!" << std::endl;
    }
    
    //找到flag, “F_GETFL” 参数说明找flag
    flag = fcntl(socket_fd,F_GETFL,0);
    //设置为非阻塞 “F_SETFL” 这个参数说明 设置
    fcntl(socket_fd,F_SETFL,flag | O_NONBLOCK);
    
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
    
    max_fd = socket_fd;//当前监听链接的socket 最开始是最大的
    for (int i = 0; i < FD_SIZE; i ++) {
        accept_fds[i] = -1;
    }
    
    
    //loop 接受链接处理数据的发送
    for(;;){
        
        FD_ZERO(&fd_sets);//清理一下容器
        FD_SET(socket_fd,&fd_sets);//把监听链接的sockt放到容器中
        
        //保存链接的socket，和设置最大for循环值
        for (int k = 0; k < maxpos; k++) {
            if (accept_fds[k] != -1) {
                if (accept_fds[k] > max_fd) {
                    max_fd = accept_fds[k];//给最大的fd赋值
                }
                FD_SET(accept_fds[k],&fd_sets);//添加新链接的socket
                
                printf("fd:%d,k:%d,max_fd:%d\n",accept_fds[k],k,max_fd);
            }
        }
        
        //遍历底层事件
        events = select(max_fd + 1,&fd_sets,NULL,NULL,NULL);
        if (events < 0) {
            perror("Failed to use select");
        }else if(events == 0){
            printf("select time out......");
        }else if(events){
            printf("events:%d\n",events);
            //判断fd_sets文件描述符中的socket_fd触发事件说明有新的链接进来
            if (FD_ISSET(socket_fd,&fd_sets)) {
               
                int a = 0;
                for (; a < FD_SIZE; a++) {
                    if (accept_fds[a] == -1) {
                        curpos = a;
                        break;
                    }
                }
                
                if (a == FD_SIZE) {
                    printf("select conntction is full!\n");
                    continue;
                }
                
                int addr_len = sizeof( struct sockaddr_in );
                //accept an new connection
                accept_fd = accept( socket_fd, (struct sockaddr *)&remote_addr, (socklen_t*)&addr_len );
                //把新链接进来的socket设置为非阻塞
                int flags = fcntl(accept_fd,F_GETFL,0);
                fcntl(accept_fd,F_GETFL,flags|O_NONBLOCK);
                
                accept_fds[curpos] = accept_fd;
               
                if (curpos+1 > maxpos) {
                    maxpos = curpos + 1;
                }
                if (accept_fd > max_fd) {
                    max_fd = accept_fd;
                }
        
                printf("new concent join fd:%d,curpos = %d",accept_fd,curpos);
            }
        }
        
        //处理数据的发送
        for (int j = 0; j < maxpos; j++) {
            if ((accept_fds[j] != -1) && FD_ISSET(accept_fds[j],&fd_sets)) {
               
                memset(in_buf,0,MESSAGE_SIZE);
                
                //receive network data and print buf
                ret = recv(accept_fds[j],(void*)in_buf,MESSAGE_SIZE,0);
                if(ret <= 0){
                    break;
                }
                
                printf("接收:%s\n",in_buf);
                ret =  send(accept_fds[j],(void*)in_buf,MESSAGE_SIZE,0);
                
                if (ret == 0) {
                    close(accept_fds[j]);
                    printf("close client connection socke:%d...\n",accept_fds[j]);
                }
            }
        }
        
        
    }
    
    if (pid != 0) {
        printf("quit server ....\n");
        close(socket_fd);
    }
    
    
    
    return 0;
}
