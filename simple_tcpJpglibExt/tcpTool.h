#ifndef _TCP_TOOL_H_
#define _TCP_TOOL_H_

#define NORMAL_PACK     0   //�������ݰ�
#define END_PACK        1   //��β���ݰ�

int tcpToolInit(void);
int SendData(unsigned char *Buf, int len, unsigned char flag);
int tcpToolEnd(void);
#endif