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
 //变量定义
int main()
{
     struct jpeg_decompress_struct cinfo;

     struct jpeg_error_mgr jerr;

     FILE * infile;

     JSAMPARRAY buffer;

     int row_stride;        

     //绑定标准错误处理结构

     cinfo.err = jpeg_std_error(&jerr);  

     //初始化JPEG对象

     jpeg_create_decompress(&cinfo);

     //指定图像文件

     if ((infile = fopen("in.jpg", "rb")) == NULL)

     {

         return;

     }

     jpeg_stdio_src(&cinfo, infile);

     //读取图像信息

     (void) jpeg_read_header(&cinfo, TRUE);

     //设定解压缩参数，此处我们将图像长宽缩小为原图的1/2

     //cinfo.scale_num=1;

     //cinfo.scale_denom=2;
     
     cinfo.out_color_space = JCS_RGB;
     //开始解压缩图像

     (void) jpeg_start_decompress(&cinfo);

     //分配缓冲区空间

     row_stride = cinfo.output_width * cinfo.output_components;

     buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

     //读取数据
     FILE *fpout;
	fpout = fopen("out.bmp","wb");
     while (cinfo.output_scanline < cinfo.output_height)

     {

         (void) jpeg_read_scanlines(&cinfo, buffer, 1);

         //output_scanline是从1开始，所以需要减1

         int line=cinfo.output_scanline-1;

	int i;
         for(i=0;i<cinfo.output_width;i++)

         {



     //bm.SetPixel(i,line,Color(255,(BYTE)buffer[0][i*3],(BYTE)buffer[0][i*3+1],(BYTE)buffer[0][i*3+2]));
		fwrite(&buffer[0][i*3+2],1,1,fpout);
		fwrite(&buffer[0][i*3+1],1,1,fpout);
		fwrite(&buffer[0][i*3],1,1,fpout);
         }
	// fwrite(buffer[0],cinfo.output_width*3,1,fpout);

     }
	fclose(fpout);
	printf("ok\n");
     //结束解压缩操作

     (void) jpeg_finish_decompress(&cinfo);

     //释放资源

     jpeg_destroy_decompress(&cinfo);

     fclose(infile);

     //在客户区绘制位图
}
