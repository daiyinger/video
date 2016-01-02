#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include "tcpTool.h"

#define PORT_NUM 10025
int sockfd = 0;
int new_fd = 0; 

int tcpToolInit(void) 
{ 
	struct sockaddr_in server_addr; 
	struct sockaddr_in client_addr; 
	int sin_size; 
	int nbytes;
	char buffer[1024];
	

	/* 服务器端开始建立sockfd描述符 */ 
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
	{ 
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* 服务器端填充 sockaddr结构 */ 
	bzero(&server_addr,sizeof(struct sockaddr_in)); // 初始化,置0
	server_addr.sin_family=AF_INET;                 // Internet
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  // (将本机器上的long数据转化为网络上的long数据)服务器程序能运行在任何ip的主机上  //INADDR_ANY 表示主机可以是任意IP地址，即服务器程序可以绑定到所有的IP上
	//server_addr.sin_addr.s_addr=inet_addr("192.168.1.1");  //用于绑定到一个固定IP,inet_addr用于把数字加格式的ip转化为整形ip
	server_addr.sin_port=htons(PORT_NUM);         // (将本机器上的short数据转化为网络上的short数据)端口号
	
	int on = 1;
	if(setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
	{
	    
	}	
	/* 捆绑sockfd描述符到IP地址 */ 
	if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
	{ 
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* 设置允许连接的最大客户端数 */ 
	if(listen(sockfd,5) == -1) 
	{ 
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	
    /* 服务器阻塞,直到客户程序建立连接 */ 
	sin_size=sizeof(struct sockaddr_in); 
   if((new_fd = accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size)) == -1) 
   	 { 
        fprintf(stderr,"Accept error:%s\n\a",strerror(errno)); 
        exit(-1); 
   	 }
} 


int SendData(unsigned char *Buf, int len, unsigned char flag)
{
    static int frame = 1;
    int ret;
    int  pos = 0;
    Buf[pos++] = 0xFF;
    Buf[pos++] = 0xFF;
    Buf[pos++] = 0x55;
    Buf[pos++] = 0xAA;
    if(flag == NORMAL_PACK)
    {
        ret = send(new_fd, Buf, len+4, 0);
    }
    else
    {
        ret = send(new_fd, Buf, 4, 0);
    }
    fprintf(stderr, " %d frame: %d\n",len, frame++);
    return ret;
}

int tcpToolEnd(void)
{
    
    /* 这个通讯已经结束 */ 
	close(new_fd);
    /* 结束通讯 */ 
	close(sockfd); 
    return 0;
}
