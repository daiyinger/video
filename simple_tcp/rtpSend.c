// NALDecoder.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "h264.h"


//NALU(Network Abstract Layer Unit)£ºÁ½±ê×¼ÖĞµÄ±ÈÌØÁ÷¶¼ÊÇÒÔNALÎªµ¥Î»£¬Ã¿¸öNALµ¥Ôª°üº¬Ò»¸öRBSP£¬
//NALUµÄÍ·ĞÅÏ¢¶¨ÒåÁËRBSPËùÊôÀàĞÍ¡£ÀàĞÍÒ»°ã°üÀ¨ĞòÁĞ²ÎÊı¼¯£¨SPS£©¡¢Í¼Ïñ²ÎÊı¼¯£¨PPS£©¡¢ÔöÇ¿ĞÅÏ¢£¨SEI£©¡¢Ìõ´ø£¨Slice£©µÈ
typedef struct
{
  int startcodeprefix_len;      //! 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
  unsigned len;                 //! Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
  unsigned max_size;            //! Nal Unit Buffer size
  int forbidden_bit;            //! should be always FALSE
  int nal_reference_idc;        //! NALU_PRIORITY_xxxx
  int nal_unit_type;            //! NALU_TYPE_xxxx    
  char *buf;                    //! contains the first byte followed by the EBSP
  unsigned short lost_packets;  //! true, if packet loss is detected
} NALU_t;

#define FILE_NAME ("./avc.h264")	//´ò¿ª264ÎÄ¼ş£¬²¢½«ÎÄ¼şÖ¸Õë¸³¸øbits,ÔÚ´ËĞŞ¸ÄÎÄ¼şÃûÊµÏÖ´ò¿ª±ğµÄ264ÎÄ¼ş¡£

FILE *bits = NULL;                //!< the bit stream file
static int FindStartCode2 (unsigned char *Buf);//²éÕÒ¿ªÊ¼×Ö·û0x000001
static int FindStartCode3 (unsigned char *Buf);//²éÕÒ¿ªÊ¼×Ö·û0x00000001
//static bool flag = true;
static int info2=0, info3=0;
RTP_FIXED_HEADER        *rtp_hdr;

NALU_HEADER		*nalu_hdr;
FU_INDICATOR	*fu_ind;
FU_HEADER		*fu_hdr;

#define portnumber 3333

//ÎªNALU_t½á¹¹Ìå·ÖÅäÄÚ´æ¿Õ¼ä
NALU_t *AllocNALU(int buffersize)
{
  NALU_t *n;

  if ((n = (NALU_t*)calloc (1, sizeof (NALU_t))) == NULL)
  {
	  printf("AllocNALU: n");
	  exit(0);
  }

  n->max_size=buffersize;

  if ((n->buf = (char*)calloc (buffersize, sizeof (char))) == NULL)
  {
    free (n);
    printf ("AllocNALU: n->buf");
	exit(0);
  }

  return n;
}
//ÊÍ·Å
void FreeNALU(NALU_t *n)
{
  if (n != NULL)
  {
    if (n->buf != NULL)
    {
      free(n->buf);
      n->buf = NULL;
    }
    free (n);
  }
}


//´ò¿ªÂëÁ÷ÎÄ¼ş
void OpenBitstreamFile (char *fn)
{
  if (NULL == (bits=fopen(fn, "rb")))
  {
	  printf("open file error\n");
	  getchar();
	  exit(0);
  }
}

//Õâ¸öº¯ÊıÊäÈëÎªÒ»¸öNAL½á¹¹Ìå£¬Ö÷Òª¹¦ÄÜÎªµÃµ½Ò»¸öÍêÕûµÄNALU²¢±£´æÔÚNALU_tµÄbufÖĞ£¬»ñÈ¡ËûµÄ³¤¶È£¬Ìî³äF,IDC,TYPEÎ»¡£
//²¢ÇÒ·µ»ØÁ½¸ö¿ªÊ¼×Ö·ûÖ®¼ä¼ä¸ôµÄ×Ö½ÚÊı£¬¼´°üº¬ÓĞÇ°×ºµÄNALUµÄ³¤¶È
int GetAnnexbNALU (NALU_t *nalu)
{
	int pos = 0;
	int StartCodeFound, rewind;
	unsigned char *Buf;
    
	if ((Buf = (unsigned char*)calloc(nalu->max_size, sizeof(char))) == NULL)
	{
		printf("GetAnnexbNALU: Could not allocate Buf memory\n");
	}

	nalu->startcodeprefix_len = 3;//³õÊ¼»¯ÂëÁ÷ĞòÁĞµÄ¿ªÊ¼×Ö·ûÎª3¸ö×Ö½Ú
  
	if (3 != fread (Buf, 1, 3, bits))//´ÓÂëÁ÷ÖĞ¶Á3¸ö×Ö½Ú
	{
		free(Buf);
		return 0;
	}
	info2 = FindStartCode2 (Buf);//ÅĞ¶ÏÊÇ·ñÎª0x000001 
	if(info2 != 1) 
	{
		//Èç¹û²»ÊÇ£¬ÔÙ¶ÁÒ»¸ö×Ö½Ú
		if(1 != fread(Buf+3, 1, 1, bits))//¶ÁÒ»¸ö×Ö½Ú
		{
			free(Buf);
			return 0;
		}
		info3 = FindStartCode3 (Buf);//ÅĞ¶ÏÊÇ·ñÎª0x00000001
		if (info3 != 1)//Èç¹û²»ÊÇ£¬·µ»Ø-1
		{ 
			free(Buf);
			return -1;
		}
		else 
		{
			//Èç¹ûÊÇ0x00000001,µÃµ½¿ªÊ¼Ç°×ºÎª4¸ö×Ö½Ú
			pos = 4;
			nalu->startcodeprefix_len = 4;
		}
   }
   else
   {
		//Èç¹ûÊÇ0x000001,µÃµ½¿ªÊ¼Ç°×ºÎª3¸ö×Ö½Ú
		nalu->startcodeprefix_len = 3;
		pos = 3;
   }
   //²éÕÒÏÂÒ»¸ö¿ªÊ¼×Ö·ûµÄ±êÖ¾Î»
   StartCodeFound = 0;
   info2 = 0;
   info3 = 0;
  
   while (!StartCodeFound)
   {
    if (feof (bits))//ÅĞ¶ÏÊÇ·ñµ½ÁËÎÄ¼şÎ²
    {
      nalu->len = (pos-1)-nalu->startcodeprefix_len;
      memcpy (nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);     
      nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
	  nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
	  nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
      free(Buf);
      return pos-1;
    }
    Buf[pos++] = fgetc (bits);//¶ÁÒ»¸ö×Ö½Úµ½BUFÖĞ
    info3 = FindStartCode3(&Buf[pos-4]);//ÅĞ¶ÏÊÇ·ñÎª0x00000001
    if(info3 != 1)
      info2 = FindStartCode2(&Buf[pos-3]);//ÅĞ¶ÏÊÇ·ñÎª0x000001
    StartCodeFound = (info2 == 1 || info3 == 1);
  }
  

 
  // Here, we have found another start code (and read length of startcode bytes more than we should
  // have.  Hence, go back in the file
  rewind = (info3 == 1)? -4 : -3;

  if (0 != fseek (bits, rewind, SEEK_CUR))//°ÑÎÄ¼şÖ¸ÕëÖ¸ÏòÇ°Ò»¸öNALUµÄÄ©Î²
  {
    free(Buf);
	printf("GetAnnexbNALU: Cannot fseek in the bit stream file");
  }

  // Here the Start code, the complete NALU, and the next start code is in the Buf.  
  // The size of Buf is pos, pos+rewind are the number of bytes excluding the next
  // start code, and (pos+rewind)-startcodeprefix_len is the size of the NALU excluding the start code

  nalu->len = (pos+rewind)-nalu->startcodeprefix_len;
  memcpy (nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);//¿½±´Ò»¸öÍêÕûNALU£¬²»¿½±´ÆğÊ¼Ç°×º0x000001»ò0x00000001
  nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
  nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
  nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
  free(Buf);
 
  return (pos+rewind);//·µ»ØÁ½¸ö¿ªÊ¼×Ö·ûÖ®¼ä¼ä¸ôµÄ×Ö½ÚÊı£¬¼´°üº¬ÓĞÇ°×ºµÄNALUµÄ³¤¶È
}
//Êä³öNALU³¤¶ÈºÍTYPE
void dump(NALU_t *n)
{
	if (!n)return;
	//printf("a new nal:");
	printf("nalu len: %d  ", n->len);
	printf("nal_unit_type: %x\n", n->nal_unit_type);
}

extern pthread_mutex_t mutex;
int rtp_init(void *arg)
{
	//my.h264 test.264 test22.264
	int argc=2;
	char* argv[2]={"","cuc_ieschool.h264"};
	
	int errno;
	NALU_t *n;							//¶¨ÒåNALU½á¹¹Ö¸Õë
	char* nalu_payload;  
	char sendbuf[1024*10];
	int sin_size; 
	int nbytes;
	int new_fd;
	struct sockaddr_in client_addr;

	
	unsigned short seq_num =0;
	int	bytes=0;
	//InitWinsock(); //³õÊ¼»¯Ì×½Ó×Ö¿â
	int    socket1;
	struct sockaddr_in server;
    int len = sizeof(server);
	float framerate = 15;
	unsigned int timestamp_increse = 0,ts_current = 0;
	timestamp_increse=(unsigned int)(90000.0 / framerate); //+0.5);

	if(argc > 1)
	{
		OpenBitstreamFile(argv[1]);	//´ò¿ª264ÎÄ¼ş£¬²¢½«ÎÄ¼şÖ¸Õë¸³¸øbits,ÔÚ´ËĞŞ¸ÄÎÄ¼şÃûÊµÏÖ´ò¿ª±ğµÄ264ÎÄ¼ş¡£
	}
	else
	{
		OpenBitstreamFile(FILE_NAME);	//´ò¿ª264ÎÄ¼ş£¬²¢½«ÎÄ¼şÖ¸Õë¸³¸øbits,ÔÚ´ËĞŞ¸ÄÎÄ¼şÃûÊµÏÖ´ò¿ª±ğµÄ264ÎÄ¼ş¡£
	}

	
	/* ·şÎñÆ÷¶Ë¿ªÊ¼½¨Á¢sockfdÃèÊö·û */ 
	if((socket1=socket(AF_INET,SOCK_STREAM,0))==-1) // AF_INET:IPV4;SOCK_STREAM:TCP
	{ 
		fprintf(stderr,"Socket error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* ·şÎñÆ÷¶ËÌî³ä sockaddr½á¹¹ */ 
	bzero(&server,sizeof(struct sockaddr_in)); // ³õÊ¼»¯,ÖÃ0
    server.sin_family=AF_INET;                 // Internet
	server.sin_addr.s_addr=htonl(INADDR_ANY);  // (½«±¾»úÆ÷ÉÏµÄlongÊı¾İ×ª»¯ÎªÍøÂçÉÏµÄlongÊı¾İ)·şÎñÆ÷³ÌĞòÄÜÔËĞĞÔÚÈÎºÎipµÄÖ÷»úÉÏ  //INADDR_ANY ±íÊ¾Ö÷»ú¿ÉÒÔÊÇÈÎÒâIPµØÖ·£¬¼´·şÎñÆ÷³ÌĞò¿ÉÒÔ°ó¶¨µ½ËùÓĞµÄIPÉÏ
	//server_addr.sin_addr.s_addr=inet_addr("192.168.1.1");  //ÓÃÓÚ°ó¶¨µ½Ò»¸ö¹Ì¶¨IP,inet_addrÓÃÓÚ°ÑÊı×Ö¼Ó¸ñÊ½µÄip×ª»¯ÎªÕûĞÎip
	server.sin_port=htons(portnumber);         // (½«±¾»úÆ÷ÉÏµÄshortÊı¾İ×ª»¯ÎªÍøÂçÉÏµÄshortÊı¾İ)¶Ë¿ÚºÅ
	
	/* À¦°ósockfdÃèÊö·ûµ½IPµØÖ· */ 
	if(bind(socket1,(struct sockaddr *)(&server),sizeof(struct sockaddr))==-1) 
	{ 
		fprintf(stderr,"Bind error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 

	/* ÉèÖÃÔÊĞíÁ¬½ÓµÄ×î´ó¿Í»§¶ËÊı */ 
	if(listen(socket1,5)==-1) 
	{ 
		fprintf(stderr,"Listen error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 
	n = AllocNALU(8000000);//Îª½á¹¹Ìånalu_t¼°Æä³ÉÔ±buf·ÖÅä¿Õ¼ä¡£·µ»ØÖµÎªÖ¸Ïònalu_t´æ´¢¿Õ¼äµÄÖ¸Õë
	

	/* ·şÎñÆ÷×èÈû,Ö±µ½¿Í»§³ÌĞò½¨Á¢Á¬½Ó */ 
	sin_size=sizeof(struct sockaddr_in); 
	if((new_fd=accept(socket1,(struct sockaddr *)(&client_addr),&sin_size))==-1) 
	{ 
		fprintf(stderr,"Accept error:%s\n\a",strerror(errno)); 
		exit(1); 
	} 
	while(1)//!feof(bits)) 
	{
		/* ¿¿¿¿¿ */
	    int res = pthread_mutex_lock(&mutex);
	    if (res)
	    {
		printf("Thread %d lock failed\n", 1);
		pthread_exit(NULL);
	    }
		GetAnnexbNALU(n);	//Ã¿Ö´ĞĞÒ»´Î£¬ÎÄ¼şµÄÖ¸ÕëÖ¸Ïò±¾´ÎÕÒµ½µÄNALUµÄÄ©Î²£¬ÏÂÒ»¸öÎ»ÖÃ¼´ÎªÏÂ¸öNALUµÄÆğÊ¼Âë0x000001
		dump(n);			//Êä³öNALU³¤¶ÈºÍTYPE
		
		memset(sendbuf,0,sizeof(sendbuf));	//Çå¿Õsendbuf£»´ËÊ±»á½«ÉÏ´ÎµÄÊ±¼ä´ÁÇå¿Õ£¬Òò´ËĞèÒªts_currentÀ´±£´æÉÏ´ÎµÄÊ±¼ä´ÁÖµ
								//rtp¹Ì¶¨°üÍ·£¬Îª12×Ö½Ú,¸Ã¾ä½«sendbuf[0]µÄµØÖ·¸³¸ørtp_hdr£¬ÒÔºó¶Ôrtp_hdrµÄĞ´Èë²Ù×÷½«Ö±½ÓĞ´Èësendbuf¡£

		rtp_hdr =(RTP_FIXED_HEADER*)&sendbuf[0]; 
		//ÉèÖÃRTP HEADER£¬
		rtp_hdr->payload     = H264;			//¸ºÔØÀàĞÍºÅ£¬
		rtp_hdr->version     = 2;				//°æ±¾ºÅ£¬´Ë°æ±¾¹Ì¶¨Îª2
		rtp_hdr->marker		 = 0;				//±êÖ¾Î»£¬ÓÉ¾ßÌåĞ­Òé¹æ¶¨ÆäÖµ¡£
        rtp_hdr->ssrc        = htonl(10);		//Ëæ»úÖ¸¶¨Îª10£¬²¢ÇÒÔÚ±¾RTP»á»°ÖĞÈ«¾ÖÎ¨Ò»
		
		//	µ±Ò»¸öNALUĞ¡ÓÚ1400×Ö½ÚµÄÊ±ºò£¬²ÉÓÃÒ»¸öµ¥RTP°ü·¢ËÍ
		if(n->len <= 1400)
		{	
			//ÉèÖÃrtp M Î»£»
			rtp_hdr->marker=1;
			rtp_hdr->seq_no     = htons(seq_num ++); //ĞòÁĞºÅ£¬Ã¿·¢ËÍÒ»¸öRTP°üÔö1
			//ÉèÖÃNALU HEADER,²¢½«Õâ¸öHEADERÌîÈësendbuf[12]
			nalu_hdr =(NALU_HEADER*)&sendbuf[12]; //½«sendbuf[12]µÄµØÖ·¸³¸ønalu_hdr£¬Ö®ºó¶Ônalu_hdrµÄĞ´Èë¾Í½«Ğ´ÈësendbufÖĞ£»
			nalu_hdr->F=n->forbidden_bit;
			nalu_hdr->NRI=n->nal_reference_idc>>5;//ÓĞĞ§Êı¾İÔÚn->nal_reference_idcµÄµÚ6£¬7Î»£¬ĞèÒªÓÒÒÆ5Î»²ÅÄÜ½«ÆäÖµ¸³¸ønalu_hdr->NRI¡£
			nalu_hdr->TYPE=n->nal_unit_type;

			nalu_payload=&sendbuf[13];//Í¬Àí½«sendbuf[13]¸³¸ønalu_payload
			memcpy(nalu_payload,n->buf+1,n->len-1);//È¥µônaluÍ·µÄnaluÊ£ÓàÄÚÈİĞ´Èësendbuf[13]¿ªÊ¼µÄ×Ö·û´®¡£
		
			ts_current=ts_current+timestamp_increse;
			rtp_hdr->timestamp=htonl(ts_current);
			bytes=n->len + 12 ;						//»ñµÃsendbufµÄ³¤¶È,ÎªnaluµÄ³¤¶È£¨°üº¬NALUÍ·µ«³ıÈ¥ÆğÊ¼Ç°×º£©¼ÓÉÏrtp_headerµÄ¹Ì¶¨³¤¶È12×Ö½Ú
			send( new_fd, sendbuf, bytes, 0 );//·¢ËÍrtp°ü
			//Sleep(100);
		}		
		else if(n->len>1400)
		{
			//µÃµ½¸ÃnaluĞèÒªÓÃ¶àÉÙ³¤¶ÈÎª1400×Ö½ÚµÄRTP°üÀ´·¢ËÍ
			int k = 0,l = 0;
			k = n->len/1400;//ĞèÒªk¸ö1400×Ö½ÚµÄRTP°ü
			l = n->len%1400;//×îºóÒ»¸öRTP°üµÄĞèÒª×°ÔØµÄ×Ö½ÚÊı
			int t = 0;//ÓÃÓÚÖ¸Ê¾µ±Ç°·¢ËÍµÄÊÇµÚ¼¸¸ö·ÖÆ¬RTP°ü
			ts_current  =ts_current+timestamp_increse;
			rtp_hdr->timestamp = htonl(ts_current);
			while(t<=k)
			{
				rtp_hdr->seq_no = htons(seq_num ++); //ĞòÁĞºÅ£¬Ã¿·¢ËÍÒ»¸öRTP°üÔö1
				if(!t)//·¢ËÍÒ»¸öĞèÒª·ÖÆ¬µÄNALUµÄµÚÒ»¸ö·ÖÆ¬£¬ÖÃFU HEADERµÄSÎ»
				{
					//ÉèÖÃrtp M Î»£»
					rtp_hdr->marker = 0;
					//ÉèÖÃFU INDICATOR,²¢½«Õâ¸öHEADERÌîÈësendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //½«sendbuf[12]µÄµØÖ·¸³¸øfu_ind£¬Ö®ºó¶Ôfu_indµÄĞ´Èë¾Í½«Ğ´ÈësendbufÖĞ£»
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc>>5;
					fu_ind->TYPE  =28;
					
					//ÉèÖÃFU HEADER,²¢½«Õâ¸öHEADERÌîÈësendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];
					fu_hdr->E = 0;
					fu_hdr->R = 0;
					fu_hdr->S = 1;
					fu_hdr->TYPE = n->nal_unit_type;
					
				
					nalu_payload = &sendbuf[14];//Í¬Àí½«sendbuf[14]¸³¸ønalu_payload
					memcpy(nalu_payload,n->buf+1,1400);//È¥µôNALUÍ·
					
					bytes = 1400+14;						//»ñµÃsendbufµÄ³¤¶È,ÎªnaluµÄ³¤¶È£¨³ıÈ¥ÆğÊ¼Ç°×ººÍNALUÍ·£©¼ÓÉÏrtp_header£¬fu_ind£¬fu_hdrµÄ¹Ì¶¨³¤¶È14×Ö½Ú
					send( new_fd, sendbuf, bytes, 0 );//·¢ËÍrtp°ü
					t++;
					
				}
				//·¢ËÍÒ»¸öĞèÒª·ÖÆ¬µÄNALUµÄ·ÇµÚÒ»¸ö·ÖÆ¬£¬ÇåÁãFU HEADERµÄSÎ»£¬Èç¹û¸Ã·ÖÆ¬ÊÇ¸ÃNALUµÄ×îºóÒ»¸ö·ÖÆ¬£¬ÖÃFU HEADERµÄEÎ»
				else if( k == t )//·¢ËÍµÄÊÇ×îºóÒ»¸ö·ÖÆ¬£¬×¢Òâ×îºóÒ»¸ö·ÖÆ¬µÄ³¤¶È¿ÉÄÜ³¬¹ı1400×Ö½Ú£¨µ±l>1386Ê±£©¡£
				{
					
					//ÉèÖÃrtp M Î»£»µ±Ç°´«ÊäµÄÊÇ×îºóÒ»¸ö·ÖÆ¬Ê±¸ÃÎ»ÖÃ1
					rtp_hdr->marker = 1;
					//ÉèÖÃFU INDICATOR,²¢½«Õâ¸öHEADERÌîÈësendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //½«sendbuf[12]µÄµØÖ·¸³¸øfu_ind£¬Ö®ºó¶Ôfu_indµÄĞ´Èë¾Í½«Ğ´ÈësendbufÖĞ£»
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc>>5;
					fu_ind->TYPE = 28;
						
					//ÉèÖÃFU HEADER,²¢½«Õâ¸öHEADERÌîÈësendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];
					fu_hdr->R = 0;
					fu_hdr->S = 0;
					fu_hdr->TYPE = n->nal_unit_type;
					fu_hdr->E = 1;

					nalu_payload = &sendbuf[14];//Í¬Àí½«sendbuf[14]µÄµØÖ·¸³¸ønalu_payload
					memcpy(nalu_payload,n->buf+t*1400+1,l-1);//½«nalu×îºóÊ£ÓàµÄl-1(È¥µôÁËÒ»¸ö×Ö½ÚµÄNALUÍ·)×Ö½ÚÄÚÈİĞ´Èësendbuf[14]¿ªÊ¼µÄ×Ö·û´®¡£
					bytes = l-1+14;		//»ñµÃsendbufµÄ³¤¶È,ÎªÊ£ÓànaluµÄ³¤¶Èl-1¼ÓÉÏrtp_header£¬FU_INDICATOR,FU_HEADERÈı¸ö°üÍ·¹²14×Ö½Ú
					send( new_fd, sendbuf, bytes, 0 );//·¢ËÍrtp°ü
					t++;
					//	Sleep(100);
				}
				else if((t < k) && (0 != t))
				{
					//ÉèÖÃrtp M Î»£»
					rtp_hdr->marker = 0;
					//ÉèÖÃFU INDICATOR,²¢½«Õâ¸öHEADERÌîÈësendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //½«sendbuf[12]µÄµØÖ·¸³¸øfu_ind£¬Ö®ºó¶Ôfu_indµÄĞ´Èë¾Í½«Ğ´ÈësendbufÖĞ£»
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc>>5;
					fu_ind->TYPE = 28;
						
					//ÉèÖÃFU HEADER,²¢½«Õâ¸öHEADERÌîÈësendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];
					//fu_hdr->E=0;
					fu_hdr->R = 0;
					fu_hdr->S = 0;
					fu_hdr->E = 0;
					fu_hdr->TYPE = n->nal_unit_type;
				
					nalu_payload = &sendbuf[14];//Í¬Àí½«sendbuf[14]µÄµØÖ·¸³¸ønalu_payload
					memcpy(nalu_payload,n->buf+t*1400+1,1400);//È¥µôÆğÊ¼Ç°×ºµÄnaluÊ£ÓàÄÚÈİĞ´Èësendbuf[14]¿ªÊ¼µÄ×Ö·û´®¡£
					bytes = 1400+14;						//»ñµÃsendbufµÄ³¤¶È,ÎªnaluµÄ³¤¶È£¨³ıÈ¥Ô­NALUÍ·£©¼ÓÉÏrtp_header£¬fu_ind£¬fu_hdrµÄ¹Ì¶¨³¤¶È14×Ö½Ú
					send( new_fd, sendbuf, bytes, 0 );//·¢ËÍrtp°ü
					t++;
				}
			}
		}
	}

	FreeNALU(n);

	printf("H264 Transport Ok!\n");
	sleep(1);

	return 0;
}

static int FindStartCode2 (unsigned char *Buf)
{
	if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 1)
	{
		return 0; //ÅĞ¶ÏÊÇ·ñÎª0x000001,Èç¹ûÊÇ·µ»Ø1
	}
	else
	{
		return 1;
	}
}

static int FindStartCode3 (unsigned char *Buf)
{
	if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 0 || Buf[3] != 1)
	{
		return 0;//ÅĞ¶ÏÊÇ·ñÎª0x00000001,Èç¹ûÊÇ·µ»Ø1
	}
	else
	{
		return 1;
	}
}
