objects = simple_video
src = simple_code.c video_yuv420.c jpeg2rgb.c
simple_video: $(src) video_yuv420.o simple_code.o fscale.o jpeg2rgb.o
	g++ simple_code.o video_yuv420.o fscale.o jpeg2rgb.o -o simple_video -L/usr/local/ffmpeg/lib -lx264 -ldl -pthread -lavformat -lavcodec -lavutil -lswscale
video_yuv420.o:video_yuv420.c
	gcc -c video_yuv420.c
simple_code.o:simple_code.c simple_code.h
	gcc simple_code.c -c  
fscale.o:fscale.c
	gcc -c fscale.c 
jpeg2rgb.o:jpeg2rgb.c
	gcc -c jpeg2rgb.c 
info:
	g++ simple_code.cpp -o simple_code -lx264 -ldl -pthread
fscale:
	gcc -o fscale fscale.c -L/usr/local/ffmpeg/lib -lavformat -lavcodec -lavutil -lswscale
clean:
	rm video_yuv420.o 
	rm -f simple_code.o
	rm -f simple_video
	rm -f simple_code
	rm -f fscale
	rm -f fscale.o
	rm -f jpeg2rgb.o
