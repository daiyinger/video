objects = simple_video
src = simple_code_info.c video_yuv420.c
simple_video: $(src) video_yuv420.o simple_code_info.o fscale.o
	g++ simple_code_info.o video_yuv420.o fscale.o -o simple_video -L/usr/local/ffmpeg/lib -lx264 -ldl -pthread -lavformat -lavcodec -lavutil -lswscale
video_yuv420.o:video_yuv420.c
	gcc -c video_yuv420.c
simple_code_info.o:simple_code_info.c simple_code_info.h
	gcc simple_code_info.c -c  
fscale.o:fscale.c
	gcc -c fscale.c 
info:
	g++ simple_code_info.cpp -o simple_code_info -lx264 -ldl -pthread
fscale:
	gcc -o fscale fscale.c -L/usr/local/ffmpeg/lib -lavformat -lavcodec -lavutil -lswscale
clean:
	rm video_yuv420.o 
	rm -f simple_code_info.o
	rm -f simple_video
	rm -f simple_code_info
	rm -f fscale
	rm -f fscale.o
