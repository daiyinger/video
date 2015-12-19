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
#include "simple_code_info.h"
#include "fscale.h"

#define SERVERPORT 6666
#define SERVERIP "192.168.1.100"


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
static VideoBuffer *buffers=NULL;
pthread_mutex_t g_lock;
pthread_cond_t g_cond;
int fd;
int g_udp_fd;
struct sockaddr_in g_addr;

//������Ƶ����ʽ����ʽ
int setMark(void)
{
		int ret;
		struct v4l2_capability cap;		//��ȡ��Ƶ�豸�Ĺ���
		struct v4l2_format fmt;			//������Ƶ�ĸ�ʽ
		//struct v4l2_control ctrl;		//

		do
		{
				ret=ioctl(fd,VIDIOC_QUERYCAP,&cap);	//��ѯ��������
		}while(ret==-1 && errno==EAGAIN);

		if(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)	//��ѯ��Ƶ�豸�Ƿ�֧������������
		{
			printf("capability is V4L2_CAP_VIDEO_CAPTURE\n");
		}
		
		memset(&fmt,0,sizeof(fmt));
		fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;//����������
		fmt.fmt.pix.width=320;//��������16�ı���
		fmt.fmt.pix.height=240;//�ߣ�������16�ı���
		//fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YVU420;//V4L2_PIX_FMT_YUYV;// ��Ƶ���ݴ洢����
		fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUV420;//=V4L2_PIX_FMT_YUYV;// ��Ƶ���ݴ洢����
		if (ioctl(fd, VIDIOC_S_FMT, &fmt) <0)	//��������ͷ��Ƶ��ʽ
		{
			printf("set format failed\n");
			return -1;
		}


}

//���������ڴ�
int AllocMem(void)
{
	int numBufs=0;
	struct v4l2_requestbuffers  req;	//�����ڴ�
	struct v4l2_buffer buf;				//V4L2��ƵBuffer

	req.count=4;											//��������
	req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;					//�������� ������
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

		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)//��ȡ������Ϣ 
		{
				printf("VIDIOC_QUERYBUF error\n");
				return -1;
		}
		buffers[numBufs].length=buf.length;
		buffers[numBufs].start=mmap(NULL,buf.length,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,fd, buf.m.offset);//ת������Ե�ַ
		if(buffers[numBufs].start==MAP_FAILED)
		{
				return -1;
		}
		if(ioctl(fd,VIDIOC_QBUF,&buf)==-1)//���뻺�����
		{
				return -1;
		}
	}
}

//������Ƶ�ɼ�
void video_on()
{
		enum v4l2_buf_type type;
		type = V4L2_BUF_TYPE_VIDEO_CAPTURE;   
		if (ioctl (fd, VIDIOC_STREAMON, &type) < 0) 
		{
				printf("VIDIOC_STREAMON error\n");
				// return -1;
		}
}
void video_off(void);
FILE *fps;
//��Ƶ�ɼ��̺߳���
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
//��Ƶ�ɼ�ѭ������
int video(int num)
{
        char name[20];
		FILE *fpi;
        struct stat bufs;
		struct v4l2_buffer buf;
		memset(&buf,0,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = 0;
        int cnt = 0;
#if 0
        sprintf(name,"pic/%d.yuv",cnt);
        if((fps = fopen(name,"wb")) == NULL)
		{
			printf("open file error!\n");
			exit(0);
		}
#endif
		
		system("rm -f out1.jpg");
            	system("rm -f tmp.yuv");
		while(num>0)
		{
		    num--;
            
			if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1)
			{
					return -1;
			}
			memcpy(databuf->buf,buffers[buf.index].start,buffers[buf.index].length);
			databuf->datasize=buf.bytesused;
            cnt++;
            if((fps = fopen("out.jpg","wb")) == NULL)
    		{
    			printf("open file error!\n");
    			exit(0);
    		}
			fwrite(databuf->buf, 1, buf.bytesused, fps);
            fclose(fps);
	        /*
            system("ffmpeg -i out.jpg -vf scale=640:480 out1.jpg >ffmpeg.log 2>&1 &");
            
            while(stat("out1.jpg", &bufs) != 0)
	        {
	            usleep(10000);
            }
	        usleep(100000);
            system("ffmpeg -i out1.jpg -s 640x480 -pix_fmt yuv420p tmp.yuv >ffmpeg.log 2>&1 &");
	   		while(stat("tmp.yuv", &bufs) != 0)
	        {
	            usleep(10000);
            }
	            usleep(100000);
			//system("cat tmp.yuv >> com.yuv");
            stat("tmp.yuv", &bufs);
            */
            system("ffmpeg -i out.jpg -s 320x240 -pix_fmt yuv420p tmp.yuv >ffmpeg.log 2>&1 &");
            
            while(stat("tmp.yuv", &bufs) != 0)
	        {
	            usleep(10000);
            }
	        usleep(100000);
            stat("tmp.yuv", &bufs);
            scale_yuv("tmp.yuv",tbuffers);
			encode_one_frame(tbuffers);
            //system("rm -f out1.jpg");
            system("rm -f tmp.yuv");
			fprintf(stderr,". ");
			//sleep(1);
			if(ioctl(fd,VIDIOC_QBUF,&buf)==-1)
			{
					return -1;
			}
		}
		fprintf(stderr,"\n");
		return 0;
}
//�ر���Ƶ�ɼ�
void video_off()
{
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    int ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if(ret ==-1)
    {
    	printf("vidio OFF error!\n");
    }
    close(fd);
}
void my_handler(int s)
{  
    printf("Caught signal %d\n",s);  
    video_off();
    exit(1);   
} 
int main(int argc,char **argv)
{
		int ret;
        int num;
        
		//signal(SIGPIPE,SIG_IGN);
//		fd=open("dev/video0",O_RDWR | O_NONBLOCK,0);
		fd = open("/dev/video0",O_RDWR,0);	//����Ƶ�豸
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
		setMark();	                            //����V4L2��ʽ
		AllocMem();	                        //�����ڴ�ӳ��

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
		pthread_video(num);
		usleep(1000);
		end_encode();
        end_scale();
		return 0;
}
