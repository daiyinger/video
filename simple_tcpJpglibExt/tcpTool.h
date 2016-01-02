#ifndef _TCP_TOOL_H_
#define _TCP_TOOL_H_

#define NORMAL_PACK     0   //正常数据包
#define END_PACK        1   //结尾数据包

int tcpToolInit(void);
int SendData(unsigned char *Buf, int len, unsigned char flag);
int tcpToolEnd(void);
#endif