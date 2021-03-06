/* 
 * Android USB Camera zc3xx Library 
 * 
 * Copyright (c) 2014  Store information technology guangzhou ltd<http://www.storeinf.com> 
 * Copyright (c) 2014  hclydao <hclydao@gmail.com> 
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License. 
 */  
#include <errno.h>  
#include <sys/types.h>      
#include <sys/stat.h>   
#include <fcntl.h>  
#include <sys/ioctl.h>  
#include <unistd.h>      
#include <stdint.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <asm/types.h>  
#include <linux/videodev2.h>  
#include <sys/mman.h>  
#include <string.h>  
#include <malloc.h>  
#include <linux/fb.h>  
//#include <jni.h>  
#include <string.h>  
//#include <android/log.h>  
#include <syslog.h>  
#include <jpeglib.h>

#define  LOG_TAG    "FimcGzsd"  
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)  
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , LOG_TAG, __VA_ARGS__)  
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , LOG_TAG, __VA_ARGS__)  
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , LOG_TAG, __VA_ARGS__)  
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , LOG_TAG, __VA_ARGS__)  
  
#define fb_width(fb) ((fb)->vi.xres)
#define fb_height(fb) ((fb)->vi.yres)
#define fb_size(fb) ((fb)->vi.xres * (fb)->vi.yres * 2)
#define  IMAGEWIDTH    640
#define  IMAGEHEIGHT   480
struct fimc_buffer {  
    unsigned char *start;  
    size_t  length;  
};  
  
static int fd = -1;  
static int fd_lowpower = -1;  
static int fd_bmp;
static int fb = -1;  
struct fimc_buffer *buffers=NULL;  
struct v4l2_buffer v4l2_buf;  
static int bufnum = 1;  
static int mwidth,mheight;  
unsigned char *previewBuffer;
unsigned char *global_buf;
unsigned char *fbmem;
static char *folder = "/mnt/sdcard/DCIM";
struct FB {
    unsigned short *bits;
    unsigned size;
    int fd;
    struct fb_fix_screeninfo fi;
    struct fb_var_screeninfo vi;
};
static struct FB fbdev;
int decode_jpeg(unsigned char *data,int datasize, int widthPtr, int heightPtr, unsigned char *buffer)  ;
int main()
{
    char buf1[1024*512];
	unsigned char buf2[1024*1024*2]; 
   	int  JpegBufSize = 0;   
	FILE *fp;

	
    //LoadJpegFile("in.jpg");   
	fp = fopen("in.jpg","rb");
	
    //get jpg file length   
    fseek(fp,0L,SEEK_END);   
    JpegBufSize=ftell(fp);   
    //rewind to the beginning of the file   
    fseek(fp,0L,SEEK_SET);   
	fread(buf1,JpegBufSize,1,fp);
	decode_jpeg(buf1,JpegBufSize,320,240,buf2);		
	FILE *fpout;
	fpout = fopen("out.bmp","wb");

	fwrite(buf2,320*240*3,1,fpout);
	fclose(fpout);
	return 0;
}
int fb_open(struct FB *fb)
{
    fb->fd = open("/dev/graphics/fb0", O_RDWR);
    if (fb->fd < 0)
        exit (EXIT_FAILURE);

    if (ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->fi) < 0)
        goto fail;
    if (ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vi) < 0)
        goto fail;

    fb->bits = mmap(0, fb_size(fb), PROT_READ | PROT_WRITE,MAP_SHARED, fb->fd, 0);
    if (fb->bits == MAP_FAILED)
        goto fail;

    return 0;

fail:
    close(fb->fd);
    exit (EXIT_FAILURE);
}
static void fb_update(struct FB *fb)
{
    fb->vi.yoffset = 1;
    ioctl(fb->fd, FBIOPUT_VSCREENINFO, &fb->vi);
    fb->vi.yoffset = 0;
    ioctl(fb->fd, FBIOPUT_VSCREENINFO, &fb->vi);
}

int write_to_screen(struct FB *fb, unsigned char *data, unsigned int width, unsigned int height)
{
    unsigned short *dst;
    unsigned short *src;
    int start;
    int pos;
    int count;
    int stride;

    if (width <= 0 || height <=0)
        exit (EXIT_FAILURE);

    if ( width > fb_width(fb) )
        goto fail_close;

    src = (unsigned short *)data;
    dst = fb->bits;
    start = 0;

    stride = fb_width(fb) - width;
    pos = start;
    count = width * height;
    while (pos < count) {
        * dst = * src;
        dst ++;
        if ( ( (pos + 1) % width ) == 0 )
            dst = dst + stride;
        pos ++;
        src ++;
    }

    fb_update(fb);
    return 0;

fail_close:
    exit (EXIT_FAILURE);
}

static inline void yuv_to_rgb16(unsigned char y,
                                unsigned char u,
                                unsigned char v,
                                unsigned char *rgb)
{
    register int r,g,b;
    int rgb16;

    r = (1192 * (y - 16) + 1634 * (v - 128) ) >> 10;
    g = (1192 * (y - 16) - 833 * (v - 128) - 400 * (u -128) ) >> 10;
    b = (1192 * (y - 16) + 2066 * (u - 128) ) >> 10;

    r = r > 255 ? 255 : r < 0 ? 0 : r;
    g = g > 255 ? 255 : g < 0 ? 0 : g;
    b = b > 255 ? 255 : b < 0 ? 0 : b;

    rgb16 = (int)(((r >> 3)<<11) | ((g >> 2) << 5)| ((b >> 3) << 0));

    *rgb = (unsigned char)(rgb16 & 0xFF);
    rgb++;
    *rgb = (unsigned char)((rgb16 & 0xFF00) >> 8);

}

void convert(unsigned char *buf, unsigned char *rgb, int width, int height)
{
    int x,y,z=0;
    int blocks;

    blocks = (width * height) * 2;

    for (y = 0; y < blocks; y+=4) {
        unsigned char Y1, Y2, U, V;

        Y1 = buf[y + 0];
        U = buf[y + 1];
        Y2 = buf[y + 2];
        V = buf[y + 3];

        yuv_to_rgb16(Y1, U, V, &rgb[y]);
        yuv_to_rgb16(Y2, U, V, &rgb[y + 2]);
    }
}

typedef unsigned char  BYTE;
typedef unsigned short	WORD;
typedef unsigned long  DWORD;
typedef struct tagBITMAPFILEHEADER{
     WORD	bfType;                // the flag of bmp, value is "BM"
     DWORD    bfSize;                // size BMP file ,unit is bytes
     DWORD    bfReserved;            // 0
     DWORD    bfOffBits;             // must be 54

}BITMAPFILEHEADER;

 
typedef struct tagBITMAPINFOHEADER{
     DWORD    biSize;                // must be 0x28
     DWORD    biWidth;           //
     DWORD    biHeight;          //
     WORD		biPlanes;          // must be 1
     WORD		biBitCount;            //
     DWORD    biCompression;         //
     DWORD    biSizeImage;       //
     DWORD    biXPelsPerMeter;   //
     DWORD    biYPelsPerMeter;   //
     DWORD    biClrUsed;             //
     DWORD    biClrImportant;        //
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
     BYTE	rgbBlue;
     BYTE	rgbGreen;
     BYTE	rgbRed;
     BYTE	rgbReserved;
}RGBQUAD;
unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];

//unsigned char *buffer;  
/*This returns an array for a 24 bit image.*/  
int decode_jpeg(unsigned char *data,int datasize, int widthPtr, int heightPtr, unsigned char *buffer)  
{  
    register JSAMPARRAY lineBuf;  
    struct jpeg_decompress_struct cinfo;  
    struct jpeg_error_mgr err_mgr;  
    int bytesPerPix;
    //u_char *retBuf;  
    register unsigned int x;
    register unsigned int y;
   
    cinfo.err = jpeg_std_error(&err_mgr);  
  
    jpeg_create_decompress(&cinfo);  
    jpeg_mem_src(&cinfo,data,datasize);
    jpeg_read_header(&cinfo, 1);  
    jpeg_start_decompress(&cinfo);
    y = 0;
    unsigned char *ptr = buffer; 
    while (cinfo.output_scanline < cinfo.output_height) 
    {
       // ptr += 320*3*y;
	jpeg_read_scanlines(&cinfo, &(ptr), 1);
	//break;
        unsigned short color;
        for (x = 0; x < cinfo.output_width; x++) 
        {
            //color = RGB888toRGB565(buffer[x * 3], buffer[x * 3 + 1], buffer[x * 3 + 2]);
            //fb_pixel(fbmem, fb_width, fb_height, x, y, color);
            //unsigned short *dst = ((unsigned short *) fb->bits + y * 800 + x);
            //*dst = color;
        }
	    y++;    // next scanline
    }
    //fb_update(fb);
    jpeg_finish_decompress(&cinfo);  
    jpeg_destroy_decompress(&cinfo);  
  
    return 0;  
}  
