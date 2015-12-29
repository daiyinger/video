#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"

#define RGB2YUV    //由RGB图像转换为YUV图像

//#define DST_W 640
//#define DST_H 480
#define DST_W 320
#define DST_H 240

#define SRC_W 320
#define SRC_H 240

#ifdef RGB2YUV
    #define SRC_PIX_FMT AV_PIX_FMT_BGR24
#else
    #define SRC_PIX_FMT AV_PIX_FMT_YUV420P  //yuv420转yuv420(只缩放)
#endif
#define DST_PIX_FMT AV_PIX_FMT_YUV420P

#define SRC_BPP 12
#define DST_BPP 12

#define RESCALE_METHOD SWS_BICUBIC

//Structures
uint8_t *src_data[4];
int src_linesize[4];

uint8_t *dst_data[4];
int dst_linesize[4];

struct SwsContext *img_convert_ctx;

int sacle_init(void)
{
    int ret = 0;

#ifndef RGB2YUV
    ret= av_image_alloc(src_data, src_linesize,SRC_W, SRC_H, SRC_PIX_FMT, 1);
	if (ret < 0) 
    {
		printf( "Could not allocate source image\n");
		return -1;
	}
#endif

	ret = av_image_alloc(dst_data, dst_linesize,DST_W, DST_H, DST_PIX_FMT, 1);
	if (ret < 0) 
    {
		printf( "Could not allocate destination image\n");
		return -1;
	}

	//Init Method 2
	img_convert_ctx = sws_getContext(SRC_W, SRC_H,SRC_PIX_FMT, DST_W, DST_H, DST_PIX_FMT, 
										RESCALE_METHOD, NULL, NULL, NULL); 
    return 0;
}

//YUV缩放
int scale_yuv(char *filename, unsigned char* dst_buffer)
{
	//Parameters
    int ret = 0;
    int pos = 0;
	FILE *src_file = NULL;

	uint8_t *temp_buffer = (uint8_t *)malloc(SRC_W*SRC_H*SRC_BPP/8);
    if(temp_buffer == NULL)
    {
        fprintf(stderr,"scale alloc mem error\n");
        return -1;
    }
	
    src_file = fopen(filename, "rb");
    if (fread(temp_buffer, 1, SRC_W*SRC_H*SRC_BPP/8, src_file) != SRC_W*SRC_H*SRC_BPP/8)
    {
        fprintf(stderr,"read yuv file error\n");
        ;
    }
    
    memcpy(src_data[0],temp_buffer,SRC_W*SRC_H);                    //Y
    memcpy(src_data[1],temp_buffer+SRC_W*SRC_H,SRC_W*SRC_H/4);      //U
    memcpy(src_data[2],temp_buffer+SRC_W*SRC_H*5/4,SRC_W*SRC_H/4);  //V

    sws_scale(img_convert_ctx,(const uint8_t * const *)src_data, src_linesize, 0, SRC_H, dst_data, dst_linesize);
     
    pos = 0;
    memcpy(dst_buffer, dst_data[0],DST_W*DST_H);            //Y
    pos += DST_W*DST_H;
    memcpy(dst_buffer+pos, dst_data[1],DST_W*DST_H/4);      //U
    pos += DST_W*DST_H/4;
    memcpy(dst_buffer+pos, dst_data[2],DST_W*DST_H/4);      //V

    free(temp_buffer);
    fclose(src_file);
	return 0;
}

// rgb24 to yuv420
int rgbToYuv420(uint8_t* RgbBuffer, unsigned char *dstBuf)  //用FFMPEG
{
    int pos;

    uint8_t *rgb_src[3] = {RgbBuffer, NULL, NULL};
    int rgb_stride[3] = {3*SRC_W, 0, 0};

    //翻转RGB
    rgb_src[0] += rgb_stride[0]*(SRC_H-1); 
    rgb_stride[0] *= -1;

    sws_scale(img_convert_ctx,(const uint8_t * const*)rgb_src, rgb_stride, 0, SRC_H,dst_data, dst_linesize);
    
    pos = 0;
    memcpy(dstBuf, dst_data[0],DST_W*DST_H);            //Y
    pos += DST_W*DST_H;
    memcpy(dstBuf+pos, dst_data[1],DST_W*DST_H/4);      //U
    pos += DST_W*DST_H/4;
    memcpy(dstBuf+pos, dst_data[2],DST_W*DST_H/4);      //V

    return 0;
}

int end_scale(void)
{
    sws_freeContext(img_convert_ctx);
	//free(temp_buffer);
#ifndef RGB2YUV
	av_freep(&src_data[0]);
#endif
	av_freep(&dst_data[0]);

    return 0;
}

/*
int main_test(int argc, char* argv[])
{
	//Parameters
	fprintf(stderr,"start !\n");
	sleep(1);
	FILE *src_file =fopen("1.yuv", "rb");
	const int SRC_W=320,SRC_H=240;
	enum AVPixelFormat SRC_PIX_FMT=AV_PIX_FMT_YUV420P;



	int SRC_BPP=av_get_bits_per_pixel(av_pix_fmt_desc_get(SRC_PIX_FMT));

	FILE *dst_file = fopen("2.yuv", "wb");
	const int DST_W=640,DST_H=480;
	enum AVPixelFormat DST_PIX_FMT=AV_PIX_FMT_YUV420P;
	int DST_BPP=av_get_bits_per_pixel(av_pix_fmt_desc_get(DST_PIX_FMT));

	//Structures
	uint8_t *src_data[4];
	int src_linesize[4];

	uint8_t *dst_data[4];
	int dst_linesize[4];

	int RESCALE_METHOD=SWS_BICUBIC;
	struct SwsContext *img_convert_ctx;
	uint8_t *temp_buffer=(uint8_t *)malloc(SRC_W*SRC_H*SRC_BPP/8);
	
	fprintf(stderr,"start 12 !\n");
	int frame_idx=0;
	int ret=0;
	ret= av_image_alloc(src_data, src_linesize,SRC_W, SRC_H, SRC_PIX_FMT, 1);
	if (ret< 0) {
		printf( "Could not allocate source image\n");
		return -1;
	}
	fprintf(stderr,"start 13 !\n");
	ret = av_image_alloc(dst_data, dst_linesize,DST_W, DST_H, DST_PIX_FMT, 1);
	if (ret< 0) {
		printf( "Could not allocate destination image\n");
		return -1;
	}

	fprintf(stderr,"start 14 !\n");
	sleep(1);

	//Init Method 2
	img_convert_ctx = sws_getContext(SRC_W, SRC_H,SRC_PIX_FMT, DST_W, DST_H, DST_PIX_FMT, 
										RESCALE_METHOD, NULL, NULL, NULL); 
	
	//Colorspace
	fprintf(stderr,"start 1 !\n");
	while(1)
	{
        printf()
		if (fread(temp_buffer, 1, SRC_W*SRC_H*SRC_BPP/8, src_file) != SRC_W*SRC_H*SRC_BPP/8)
		{
			break;
		}
		
		switch(SRC_PIX_FMT)
		{
			case AV_PIX_FMT_GRAY8:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H);
				break;
			}
			case AV_PIX_FMT_YUV420P:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H);                    //Y
				memcpy(src_data[1],temp_buffer+SRC_W*SRC_H,SRC_W*SRC_H/4);      //U
				memcpy(src_data[2],temp_buffer+SRC_W*SRC_H*5/4,SRC_W*SRC_H/4);  //V
				break;
			}
			case AV_PIX_FMT_YUV422P:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H);                    //Y
				memcpy(src_data[1],temp_buffer+SRC_W*SRC_H,SRC_W*SRC_H/2);      //U
				memcpy(src_data[2],temp_buffer+SRC_W*SRC_H*3/2,SRC_W*SRC_H/2);  //V
				break;
			}
			case AV_PIX_FMT_YUV444P:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H);                    //Y
				memcpy(src_data[1],temp_buffer+SRC_W*SRC_H,SRC_W*SRC_H);        //U
				memcpy(src_data[2],temp_buffer+SRC_W*SRC_H*2,SRC_W*SRC_H);      //V
				break;
			}
			case AV_PIX_FMT_YUYV422:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H*2);                  //Packed
				break;
			}
			case AV_PIX_FMT_RGB24:
			{
				memcpy(src_data[0],temp_buffer,SRC_W*SRC_H*3);                  //Packed
				break;
			}
			default:
			{
				printf("Not Support Input Pixel Format.\n");
				break;
			}
		}
		
		sws_scale(img_convert_ctx,(const uint8_t * const *)src_data, src_linesize, 0, SRC_H, dst_data, dst_linesize);
		printf("Finish process frame %5d\n",frame_idx);
		frame_idx++;

		switch(DST_PIX_FMT){
		case AV_PIX_FMT_GRAY8:{
			fwrite(dst_data[0],1,DST_W*DST_H,dst_file);	
			break;
							  }
		case AV_PIX_FMT_YUV420P:{
			fwrite(dst_data[0],1,DST_W*DST_H,dst_file);                 //Y
			fwrite(dst_data[1],1,DST_W*DST_H/4,dst_file);               //U
			fwrite(dst_data[2],1,DST_W*DST_H/4,dst_file);               //V
			break;
								}
		case AV_PIX_FMT_YUV422P:{
			fwrite(dst_data[0],1,DST_W*DST_H,dst_file);					//Y
			fwrite(dst_data[1],1,DST_W*DST_H/2,dst_file);				//U
			fwrite(dst_data[2],1,DST_W*DST_H/2,dst_file);				//V
			break;
								}
		case AV_PIX_FMT_YUV444P:{
			fwrite(dst_data[0],1,DST_W*DST_H,dst_file);                 //Y
			fwrite(dst_data[1],1,DST_W*DST_H,dst_file);                 //U
			fwrite(dst_data[2],1,DST_W*DST_H,dst_file);                 //V
			break;
								}
		case AV_PIX_FMT_YUYV422:{
			fwrite(dst_data[0],1,DST_W*DST_H*2,dst_file);               //Packed
			break;
								}
		case AV_PIX_FMT_RGB24:{
			fwrite(dst_data[0],1,DST_W*DST_H*3,dst_file);               //Packed
			break;
							  }
		default:{
			printf("Not Support Output Pixel Format.\n");
			break;
							}
		}
	}

	sws_freeContext(img_convert_ctx);

	free(temp_buffer);
	fclose(dst_file);
	av_freep(&src_data[0]);
	av_freep(&dst_data[0]);

	return 0;
}
*/
