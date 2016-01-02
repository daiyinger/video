/**
* YUV格式的像素数据编码为H.264码流
* 基于libx264的视频编码器
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#include "simple_code.h"
#if defined ( __cplusplus)
extern "C"
{
#include "x264.h"
};
#else
#include "x264.h"
#endif

#include "tcpTool.h"
 

FILE* fp_dst = NULL;
x264_param_t* pParam = NULL;
x264_picture_t* pPic_in = NULL;
x264_picture_t* pPic_out = NULL;
x264_nal_t* pNals = NULL;
x264_t* pHandle   = NULL;
int frames = 0;
int y_size = 0;
int iNal   = 0;
 
int encode_init(void)
{
 
	int ret = 0;
	//int cnt = 0; 

	if((fp_dst = fopen("cuc_ieschool.h264", "wb")) == NULL)
	{
		fprintf(stderr, "open 264 file!\n");
		return -1;
	}

	int csp = X264_CSP_I420;
	//int width = 640,height = 480;
	int width=320,height=240;
 


	pPic_in = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	pPic_out = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	pParam = (x264_param_t*)malloc(sizeof(x264_param_t));

	//x264_param_default(pParam);
	x264_param_default_preset(pParam, "fast", "zerolatency");
	pParam->i_width   = width;
	pParam->i_height  = height;

	
	pParam->i_log_level  = X264_LOG_WARNING;//X264_LOG_DEBUG;
	pParam->i_fps_num = 10;
	pParam->i_csp=csp;

	x264_param_apply_profile(pParam, x264_profile_names[5]);

	pHandle = x264_encoder_open(pParam);

	x264_picture_init(pPic_out);
	x264_picture_alloc(pPic_in, csp, pParam->i_width, pParam->i_height);

	//ret = x264_encoder_headers(pHandle, &pNals, &iNal);
	y_size = pParam->i_width * pParam->i_height;
	frames = 0;
	return 0;
}

int encode_one_frame(unsigned char *src_data, unsigned char *dst_data, int *retLen)
{
	int ret = 0;
	int j = 0;
	int pos = 0;

	memcpy(pPic_in->img.plane[0], src_data, y_size);     //Y
	pos += y_size;
	memcpy(pPic_in->img.plane[1], src_data+pos, y_size/4);     //U
	pos += y_size/4;
	memcpy(pPic_in->img.plane[2], src_data+pos, y_size/4);     //V

	pPic_in->i_pts = frames;

	ret = x264_encoder_encode(pHandle, &pNals, &iNal, pPic_in, pPic_out);
	if (ret < 0)
	{
		fprintf(stderr,"Error.\n");
		return -1;
	}
    int pos1 = 0;
	for (j = 0; j < iNal; ++j)
	{
#ifdef WRITE_TO_FILE	    
		fwrite(pNals[j].p_payload,pNals[j].i_payload,1,fp_dst);
#else
        if(dst_data != NULL)
		{
            memcpy(dst_data+pos1,pNals[j].p_payload,pNals[j].i_payload);
		    pos1 += pNals[j].i_payload;
        }
#endif
	}
    *retLen = pos1;
	frames++;
	return ret;
}

int end_encode(void)
{
	int cnt = 0;
	int i = 0, j =0;
	int ret = 0;

	//if((fp_dst == NULL) || (pPic_in == NULL)|| (pPic_out == NULL) || (pHandle == NULL) || (pNals == NULL))
	if((fp_dst != NULL) && (pPic_out != NULL) && (pHandle != NULL) /*&& (pNals != NULL)*/)
	{
		while(1)
		{
			ret = x264_encoder_encode(pHandle, &pNals, &iNal, NULL, pPic_out);
			if(ret == 0)
			{
				break;
			}
			printf("Flush 1 frame. i=%d cnt=%d\n",i,cnt++);
			for (j = 0; j < iNal; ++j)
			{
#ifdef WRITE_TO_FILE	    
				fwrite(pNals[j].p_payload,pNals[j].i_payload,1,fp_dst);
#else
				//memcpy(bufs+pos1,pNals[j].p_payload,pNals[j].i_payload);
				//pos1 += pNals[j].i_payload;
#endif
			}
			i++;
	    }
    }
    else
    {
	    ret = -1;
    }

    if(pPic_in)
    {
	    x264_picture_clean(pPic_in);
    }

    if(pHandle)
    {
	    x264_encoder_close(pHandle);
    }
    pHandle = NULL;

    printf("==============end================\n"); 
    if(pPic_in != NULL)
    {
	    free(pPic_in);
    }
    if(pPic_out != NULL)
    {
	    free(pPic_out);
    }
    if(pParam != NULL)
    {
	    free(pParam);
    }
    if(fp_dst != NULL)
    {
	    fclose(fp_dst);
    }

    return ret;
}

