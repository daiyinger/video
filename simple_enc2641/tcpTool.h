#ifndef _TCP_TOOL_H_
#define _TCP_TOOL_H_

int tcpToolInit(void);
int SendData(unsigned char *Buf, int len);
int tcpToolEnd(void);
#endif