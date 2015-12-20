#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"

int RGB24ToYUV420(int biWidth,int biHeight,uint8_t* RgbBuffer, AVFrame *pAVFrame)//第二种方法 用FFMPEG
{
    uint8_t *rgb_src[3]= {RgbBuffer, NULL, NULL};
    int rgb_stride[3]={3*biWidth, 0, 0};
    AVFrame picture;
    FILE *fps;
    struct SwsContext *yuvContext;

    fps = fopen("jpeg2-bmp.yuv","wb");
    avpicture_alloc((AVPicture*)(&picture), AV_PIX_FMT_YUV420P,biWidth*2,biHeight*2);

    yuvContext = sws_getContext(biWidth,biHeight,AV_PIX_FMT_BGR24,biWidth*2,
                                biHeight*2,AV_PIX_FMT_YUV420P,SWS_BICUBIC, NULL, NULL, NULL);

    sws_scale(yuvContext,(const uint8_t * const*)rgb_src, rgb_stride, 0, biHeight, picture.data, picture.linesize);
    *pAVFrame = picture;

    fwrite((uint8_t *)picture.data[0],(int)(320*2)*(240*2),1,fps);  
    fwrite((uint8_t *)picture.data[1],(int)(320*2)*(240*2)/4,1,fps);  
    fwrite((uint8_t *)picture.data[2],(int)(320*2)*(240*2)/4,1,fps); 
    fclose(fps);
}

int main(void)
{
   char rgbbuf[1024*512];
   AVFrame yuvbuf;
   FILE *fp,*fp1;
   fp = fopen("jpeg2-bmp.bmp","rb");
   fread(rgbbuf,1,54,fp);
   fread(rgbbuf,1,320*240*3,fp);
   fp1 = fopen("jpeg2-bmp.yuv","wb");
   RGB24ToYUV420(320,240,rgbbuf,&yuvbuf);
   //fwrite(yuvbuf,1,320*240*3,fp1);

   fclose(fp1);
   return 0;
}