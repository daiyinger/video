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
#include "jpeg2rgb.h"


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
pthread_mutex_t mutex; 
int fd;

//ÉèÖÃÊÓÆµµÄÖÆÊ½¼°¸ñÊ½
int setMark(void)
{
    int ret;
    struct v4l2_capability cap;		//»ñÈ¡ÊÓÆµÉè±¸µÄ¹¦ÄÜ
    struct v4l2_format fmt;			//ÉèÖÃÊÓÆµµÄ¸ñÊ½

    do
    {
        ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);	//²éÑ¯Çı¶¯¹¦ÄÜ
    }while(ret == -1 && errno == EAGAIN);

    if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)	//²éÑ¯ÊÓÆµÉè±¸ÊÇ·ñÖ§³ÖÊı¾İÁ÷ÀàĞÍ
    {
        printf("capability is V4L2_CAP_VIDEO_CAPTURE\n");
    }
    
    memset(&fmt,0,sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//Êı¾İÁ÷ÀàĞÍ
    fmt.fmt.pix.width = 320;//¿í£¬±ØĞëÊÇ16µÄ±¶Êı
    fmt.fmt.pix.height = 240;//¸ß£¬±ØĞëÊÇ16µÄ±¶Êı
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;//=V4L2_PIX_FMT_YUYV;// ÊÓÆµÊı¾İ´æ´¢ÀàĞÍ
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) <0)	//ÉèÖÃÉãÏñÍ·ÊÓÆµ¸ñÊ½
    {
        printf("set format failed\n");
        return -1;
    }
    return 0;
}

//ÉêÇëÎïÀíÄÚ´æ
int AllocMem(void)
{
	int numBufs=0;
	struct v4l2_requestbuffers  req;	//·ÖÅäÄÚ´æ
	struct v4l2_buffer buf;				//V4L2ÊÓÆµBuffer

	req.count=4;											//»º´æÊıÁ¿
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;					//»º´æÀàĞÍ Êı¾İÁ÷
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

		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)//¶ÁÈ¡»º´æĞÅÏ¢ 
		{
            printf("VIDIOC_QUERYBUF error\n");
            return -1;
		}
		buffers[numBufs].length = buf.length;
		buffers[numBufs].start = mmap(NULL,buf.length,
						            PROT_READ | PROT_WRITE,
						            MAP_SHARED,fd, buf.m.offset);//×ª»»³ÉÏà¶ÔµØÖ·
		if(buffers[numBufs].start == MAP_FAILED)
		{
			return -1;
		}
		if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)//·ÅÈë»º´æ¶ÓÁĞ
		{
			return -1;
		}
	}
}

//¿ªÆôÊÓÆµ²É¼¯
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
//ÊÓÆµ²É¼¯Ïß³Ìº¯Êı
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
//ÊÓÆµ²É¼¯Ñ­»·º¯Êı
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
        jpg2rgb(databuf->buf, buf.bytesused, rgbBuffers); 
        fprintf(stderr,"-");
        rgbToYuv420(rgbBuffers,tbuffers);
        encode_one_frame(tbuffers);
	/* ¿¿¿¿¿ */
	pthread_mutex_unlock(&mutex);
        if(ioctl(fd,VIDIOC_QBUF,&buf) == -1)
        {
            return -1;
        }
    }
    fprintf(stderr,"\n");
    return 0;
}
//¹Ø±ÕÊÓÆµ²É¼¯
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
    exit(1);   
}
extern int rtp_init(void *arg);
int main(int argc,char **argv)
{
    int ret;
    int num;
    pthread_t thd;
    
    /* ¿¿¿¿¿¿ */
    pthread_mutex_init(&mutex, NULL);
    //signal(SIGPIPE,SIG_IGN);
    //fd=open("dev/video0",O_RDWR | O_NONBLOCK,0);
    fd = open("/dev/video0",O_RDWR,0);	//´ò¿ªÊÓÆµÉè±¸
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
    setMark();	                            //ÉèÖÃV4L2¸ñÊ½
    AllocMem();	                        //ÉèÖÃÄÚ´æÓ³Éä

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
    pthread_create(&thd,NULL,rtp_init,NULL);
    pthread_video(num);
    usleep(1000);
    end_encode();
    end_scale();
    return 0;
}
