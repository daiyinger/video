#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <linux/videodev2.h>
#include <asm/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include "simple_code.h"
#include "fscale.h"
//#include "jpeg2rgb.h"
#include "tcpTool.h"
#include "tinyjpeg.h"

typedef struct VideoBuffer
{
    void *start;
    size_t length;
}VideoBuffer;
typedef struct data
{
    unsigned int datasize;
    char buf[];
}buf_t;
buf_t *databuf;
static VideoBuffer *buffers = NULL;
pthread_mutex_t g_lock;
pthread_cond_t g_cond;
int fd;

//设置视频的制式及格式
int setMark(void)
{
    int ret;
    struct v4l2_capability cap;		//获取视频设备的功能
    struct v4l2_format fmt;			//设置视频的格式

    do
    {
        ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);	//查询驱动功能
    }while(ret == -1 && errno == EAGAIN);

    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)	//查询视频设备是否支持数据流类型
    {
        printf("capability is V4L2_CAP_VIDEO_CAPTURE\n");
    }
    
    memset(&fmt,0,sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//数据流类型
    fmt.fmt.pix.width = 320;//宽，必须是16的倍数
    fmt.fmt.pix.height = 240;//高，必须是16的倍数
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;//=V4L2_PIX_FMT_YUYV;// 视频数据存储类型
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) <0)	//设置摄像头视频格式
    {
        printf("set format failed\n");
        return -1;
    }
    return 0;
}

//申请物理内存
int AllocMem(void)
{
	int numBufs=0;
	struct v4l2_requestbuffers  req;	//分配内存
	struct v4l2_buffer buf;				//V4L2视频Buffer

	req.count=4;											//缓存数量
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;					//缓存类型 数据流
	req.memory=V4L2_MEMORY_MMAP;							//
	buffers=calloc(req.count,sizeof(VideoBuffer));

	if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) 
	{
		return -1;
	}

	for(numBufs=0;numBufs<req.count;numBufs++)
	{
		memset(&buf,0,sizeof(buf));

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = numBufs;

		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)//读取缓存信息 
		{
            printf("VIDIOC_QUERYBUF error\n");
            return -1;
		}
		buffers[numBufs].length = buf.length;
		buffers[numBufs].start = mmap(NULL,buf.length,
						            PROT_READ | PROT_WRITE,
						            MAP_SHARED,fd, buf.m.offset);//转换成相对地址
		if(buffers[numBufs].start == MAP_FAILED)
		{
			return -1;
		}
		if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)//放入缓存队列
		{
			return -1;
		}
	}
}

//开启视频采集
int video_on()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   
    if (ioctl (fd, VIDIOC_STREAMON, &type) < 0) 
    {
        printf("VIDIOC_STREAMON error\n");
        return -1;
    }
}
void video_off(void);
FILE *fps;
//视频采集线程函数
void *pthread_video(int arg)
{
    //pthread_detach(pthread_self());
    video_on();
    databuf = (buf_t *)malloc(sizeof(buf_t)+buffers[0].length);
    video(arg);
    video_off();

    return NULL;
}
unsigned char tbuffers[1024*1024];
unsigned char rgbBuffers[1024*1024];
//视频采集循环函数
int video(int num)
{
    char name[20];
    struct v4l2_buffer buf;
    memset(&buf,0,sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int cnt = 0;
    
    while(num > 0)
    {
        num--;
        
        if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
        {
            return -1;
        }
        memcpy(databuf->buf, buffers[buf.index].start, buffers[buf.index].length);
        databuf->datasize = buf.bytesused;

        cnt++;
        fprintf(stderr,". ");
	clock_t clockStart = clock();
        if(jpg2yuv(databuf->buf, buf.bytesused, tbuffers) != 0)
	{
	    fprintf(stderr,"jpg2rgb error!\n");
	    continue;
	} 
        fprintf(stderr,"- %d ",(unsigned int)(clock()-clockStart));
        //rgbToYuv420(rgbBuffers,tbuffers);
        encode_one_frame(tbuffers);
        //encode_one_frame(tbuffers);
        if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)
        {
            return -1;
        }
    }
    fprintf(stderr,"\n");
    return 0;
}
//关闭视频采集
void video_off()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    int ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if(ret == -1)
    {
    	printf("vidio OFF error!\n");
    }
	 if(buffers != NULL)
	  {
		free(buffers);
	  }
    close(fd);
}
void my_handler(int s)
{  
    printf("Caught signal %d\n",s);  
    video_off();
    tcpToolEnd();
    exit(1);   
} 
int main(int argc,char **argv)
{
    int ret;
    int num;
    
    //signal(SIGPIPE,SIG_IGN);
    //fd=open("dev/video0",O_RDWR | O_NONBLOCK,0);
    fd = open("/dev/video0",O_RDWR,0);	//打开视频设备
    if(fd == -1)
    {
        perror("open");
        return 0;
    }
        
    struct sigaction sigIntHandler;  

    sigIntHandler.sa_handler = my_handler;  
    sigemptyset(&sigIntHandler.sa_mask);  
    sigIntHandler.sa_flags = 0;  

    sigaction(SIGINT, &sigIntHandler, NULL);  
    setMark();	                            //设置V4L2格式
    AllocMem();	                        //设置内存映射

    if(argc > 1)
    {
        num = atoi(argv[1]);
    }
    else
    {
        num = 1;
    }
    if(encode_init() != 0)
    {
        fprintf(stderr,"encode_init error \n");
    }
    if(sacle_init() != 0)
    {
        fprintf(stderr,"sacle_init error \n");
    }
    tcpToolInit();
    pthread_video(num);
    usleep(1000);
    end_encode();
    end_scale();
    tcpToolEnd();
    return 0;
}
