#ifndef _FSCALE_H_
#define _FSCALE_H_

extern int sacle_init(void);
extern int scale_yuv(char *filename, unsigned char* dst_buffer);
extern int end_scale(void);

#endif