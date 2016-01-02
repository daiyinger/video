objects = simple_video
src = simple_code.c video_main.c tcpTool.c tcpTool.h
simple_video: $(src) video_main.o simple_code.o fscale.o tcpTool.o tinyjpeg.o jidctflt.o
	g++ simple_code.o video_main.o fscale.o tinyjpeg.o jidctflt.o tcpTool.o -o simple_video -L/usr/local/ffmpeg/lib -lx264 -ldl -lpthread -lavformat -lavcodec -lavutil -lswscale
video_main.o:video_main.c 
	gcc -c video_main.c
simple_code.o:simple_code.c simple_code.h
	gcc simple_code.c -c  
fscale.o:fscale.c
	gcc -c fscale.c 
tinyjpeg.o: tinyjpeg.c tinyjpeg.h tinyjpeg-internal.h 
	gcc -c tinyjpeg.c
jidctflt.o: jidctflt.c tinyjpeg.h
	gcc -c jidctflt.c
jpeg2rgb.o:#jpeg2rgb.c
	gcc -c jpeg2rgb.c 
tcpTool.o:tcpTool.c
	gcc -c tcpTool.c
info:
	g++ simple_code.cpp -o simple_code -lx264 -ldl -pthread
fscale:
	gcc -o fscale fscale.c -L/usr/local/ffmpeg/lib -lavformat -lavcodec -lavutil -lswscale
clean:
	rm -f video_main.o 
	rm -f simple_code.o
	rm -f simple_video
	rm -f simple_code
	rm -f fscale
	rm -f fscale.o
	rm -f jpeg2rgb.o
	rm -f tinyjpeg.o jidctflt.o
