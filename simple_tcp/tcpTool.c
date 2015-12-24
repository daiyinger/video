#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

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
	

	/* �������˿�ʼ����sockfd������ */ 
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
	{ 
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* ����������� sockaddr�ṹ */ 
	bzero(&server_addr,sizeof(struct sockaddr_in)); // ��ʼ��,��0
	server_addr.sin_family=AF_INET;                 // Internet
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  // (���������ϵ�long����ת��Ϊ�����ϵ�long����)�������������������κ�ip��������  //INADDR_ANY ��ʾ��������������IP��ַ����������������԰󶨵����е�IP��
	//server_addr.sin_addr.s_addr=inet_addr("192.168.1.1");  //���ڰ󶨵�һ���̶�IP,inet_addr���ڰ����ּӸ�ʽ��ipת��Ϊ����ip
	server_addr.sin_port=htons(PORT_NUM);         // (���������ϵ�short����ת��Ϊ�����ϵ�short����)�˿ں�
	
	int on = 1;
	if(setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)))
	{
	    
	}	
	/* ����sockfd��������IP��ַ */ 
	if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1) 
	{ 
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* �����������ӵ����ͻ����� */ 
	if(listen(sockfd,5) == -1) 
	{ 
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	
    /* ����������,ֱ���ͻ����������� */ 
    sin_size=sizeof(struct sockaddr_in); 
    if((new_fd = accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size)) == -1) 
    { 
        fprintf(stderr,"Accept error:%s\n\a",strerror(errno)); 
        exit(-1); 
    }
} 

unsigned char tmpBuf[1024*100];
int SendData(unsigned char *Buf, int len)
{
    int ret;
    tmpBuf[0] = 0xFF;
    tmpBuf[1] = 0xFF;
    tmpBuf[2] = 0x55;
    tmpBuf[3] = 0xAA;
    memcpy(tmpBuf+4,Buf,len);
    ret = send(new_fd, tmpBuf, len+4, 0);
    fprintf(stderr," %d\n",len);
    return ret;
}

int tcpToolEnd(void)
{
    
    /* ���ͨѶ�Ѿ����� */ 
	close(new_fd);
    /* ����ͨѶ */ 
	close(sockfd); 
    return 0;
}
